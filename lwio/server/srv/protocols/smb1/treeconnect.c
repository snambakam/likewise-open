/* Editor Settings: expandtabs and use 4 spaces for indentation
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 * -*- mode: c, c-basic-offset: 4 -*- */

/*
 * Copyright Likewise Software
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.  You should have received a copy of the GNU General
 * Public License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * LIKEWISE SOFTWARE MAKES THIS SOFTWARE AVAILABLE UNDER OTHER LICENSING
 * TERMS AS WELL.  IF YOU HAVE ENTERED INTO A SEPARATE LICENSE AGREEMENT
 * WITH LIKEWISE SOFTWARE, THEN YOU MAY ELECT TO USE THE SOFTWARE UNDER THE
 * TERMS OF THAT SOFTWARE LICENSE AGREEMENT INSTEAD OF THE TERMS OF THE GNU
 * GENERAL PUBLIC LICENSE, NOTWITHSTANDING THE ABOVE NOTICE.  IF YOU
 * HAVE QUESTIONS, OR WISH TO REQUEST A COPY OF THE ALTERNATE LICENSING
 * TERMS OFFERED BY LIKEWISE SOFTWARE, PLEASE CONTACT LIKEWISE SOFTWARE AT
 * license@likewisesoftware.com
 */

#include "includes.h"

static
NTSTATUS
SrvBuildTreeConnectResponse(
    PLWIO_SRV_CONNECTION pConnection,
    PSMB_PACKET         pSmbRequest,
    PLWIO_SRV_TREE       pTree,
    PSMB_PACKET*        ppSmbResponse
    );

static
NTSTATUS
SrvGetServiceName(
    PSRV_SHARE_INFO pShareInfo,
    PSTR* ppszService
    );

static
NTSTATUS
SrvGetNativeFilesystem(
    PSRV_SHARE_INFO pShareInfo,
    PWSTR* ppwszNativeFilesystem
    );

NTSTATUS
SrvProcessTreeConnectAndX(
    IN  PLWIO_SRV_CONNECTION pConnection,
    IN  PSMB_PACKET          pSmbRequest,
    OUT PSMB_PACKET*         ppSmbResponse
    )
{
    NTSTATUS ntStatus = 0;
    PSMB_PACKET pSmbResponse = NULL;
    PLWIO_SRV_SESSION pSession = NULL;
    PLWIO_SRV_TREE pTree = NULL;
    BOOLEAN       bRemoveTreeFromSession = FALSE;
    PSRV_SHARE_INFO pShareInfo = NULL;
    ULONG ulOffset = 0;
    TREE_CONNECT_REQUEST_HEADER* pRequestHeader = NULL; // Do not free
    uint8_t* pszPassword = NULL; // Do not free
    uint8_t* pszService = NULL; // Do not free
    PWSTR    pwszPath = NULL; // Do not free
    PWSTR    pwszSharename = NULL;
    BOOLEAN  bInLock = FALSE;

    ntStatus = SrvConnectionFindSession(
                    pConnection,
                    pSmbRequest->pSMBHeader->uid,
                    &pSession);
    BAIL_ON_NT_STATUS(ntStatus);

    ulOffset = (PBYTE)pSmbRequest->pParams - (PBYTE)pSmbRequest->pSMBHeader;

    ntStatus = UnmarshallTreeConnectRequest(
                    pSmbRequest->pParams,
                    pSmbRequest->pNetBIOSHeader->len - ulOffset,
                    ulOffset,
                    &pRequestHeader,
                    &pszPassword,
                    &pwszPath,
                    &pszService);
    BAIL_ON_NT_STATUS(ntStatus);

    if (pRequestHeader->flags & 0x1)
    {
        NTSTATUS ntStatus2 = 0;

        ntStatus2 = SrvSessionRemoveTree(
                        pSession,
                        pSmbRequest->pSMBHeader->tid);
        if (ntStatus2)
        {
            LWIO_LOG_ERROR("Failed to remove tid [%u] from session [uid=%u]. [code:%d]",
                            pSmbRequest->pSMBHeader->tid,
                            pSmbRequest->pSMBHeader->uid,
                            ntStatus2);
        }
    }

    LWIO_LOCK_RWMUTEX_SHARED(bInLock, &pConnection->pHostinfo->mutex);

    ntStatus = SrvGetShareName(
                    pConnection->pHostinfo->pszHostname,
                    pConnection->pHostinfo->pszDomain,
                    pwszPath,
                    &pwszSharename);
    BAIL_ON_NT_STATUS(ntStatus);

    LWIO_UNLOCK_RWMUTEX(bInLock, &pConnection->pHostinfo->mutex);

    ntStatus = SrvShareFindByName(
                    pConnection->pShareList,
                    pwszSharename,
                    &pShareInfo);
    if (ntStatus == STATUS_NOT_FOUND) {
        ntStatus = STATUS_BAD_NETWORK_NAME;
    }
    BAIL_ON_NT_STATUS(ntStatus);

    ntStatus = SrvSessionCreateTree(
                    pSession,
                    pShareInfo,
                    &pTree);
    BAIL_ON_NT_STATUS(ntStatus);

    bRemoveTreeFromSession = TRUE;

    ntStatus = SrvBuildTreeConnectResponse(
                    pConnection,
                    pSmbRequest,
                    pTree,
                    &pSmbResponse);
    BAIL_ON_NT_STATUS(ntStatus);

    *ppSmbResponse = pSmbResponse;

cleanup:

    LWIO_UNLOCK_RWMUTEX(bInLock, &pConnection->pHostinfo->mutex);

    if (pSession)
    {
        SrvSessionRelease(pSession);
    }

    if (pTree)
    {
        SrvTreeRelease(pTree);
    }

    if (pShareInfo)
    {
        SrvShareReleaseInfo(pShareInfo);
    }

    if (pwszSharename)
    {
        SrvFreeMemory(pwszSharename);
    }

    return (ntStatus);

error:

    *ppSmbResponse = NULL;

    if (bRemoveTreeFromSession)
    {
        NTSTATUS ntStatus2 = 0;

        ntStatus2 = SrvSessionRemoveTree(
                        pSession,
                        pSmbRequest->pSMBHeader->tid);
        if (ntStatus2)
        {
            LWIO_LOG_ERROR("Failed to remove tid [%u] from session [uid=%u][code:%d]",
                            pSmbRequest->pSMBHeader->tid,
                            pSmbRequest->pSMBHeader->uid,
                            ntStatus2);
        }
    }

    if (pSmbResponse)
    {
        SMBPacketFree(
            pConnection->hPacketAllocator,
            pSmbResponse);
    }

    goto cleanup;
}

static
NTSTATUS
SrvBuildTreeConnectResponse(
    PLWIO_SRV_CONNECTION pConnection,
    PSMB_PACKET         pSmbRequest,
    PLWIO_SRV_TREE       pTree,
    PSMB_PACKET*        ppSmbResponse
    )
{
    NTSTATUS ntStatus = 0;
    PSMB_PACKET pSmbResponse = NULL;
    PTREE_CONNECT_RESPONSE_HEADER pResponseHeader = NULL;
    ULONG  packetByteCount = 0;
    PSTR   pszService = NULL;
    PWSTR  pwszNativeFileSystem = NULL;

    ntStatus = SMBPacketAllocate(
                    pConnection->hPacketAllocator,
                    &pSmbResponse);
    BAIL_ON_NT_STATUS(ntStatus);

    ntStatus = SMBPacketBufferAllocate(
                    pConnection->hPacketAllocator,
                    64 * 1024,
                    &pSmbResponse->pRawBuffer,
                    &pSmbResponse->bufferLen);
    BAIL_ON_NT_STATUS(ntStatus);

    ntStatus = SMBPacketMarshallHeader(
                pSmbResponse->pRawBuffer,
                pSmbResponse->bufferLen,
                COM_TREE_CONNECT_ANDX,
                0,
                TRUE,
                pTree->tid,
                pSmbRequest->pSMBHeader->pid,
                pSmbRequest->pSMBHeader->uid,
                pSmbRequest->pSMBHeader->mid,
                pConnection->serverProperties.bRequireSecuritySignatures,
                pSmbResponse);
    BAIL_ON_NT_STATUS(ntStatus);

    pSmbResponse->pSMBHeader->wordCount = 7;

    pResponseHeader = (PTREE_CONNECT_RESPONSE_HEADER)pSmbResponse->pParams;
    pSmbResponse->pData = pSmbResponse->pParams + sizeof(TREE_CONNECT_RESPONSE_HEADER);
    pSmbResponse->bufferUsed += sizeof(TREE_CONNECT_RESPONSE_HEADER);

    ntStatus = SrvGetMaximalShareAccessMask(
                    pTree->pShareInfo,
                    &pResponseHeader->maximalShareAccessMask);
    BAIL_ON_NT_STATUS(ntStatus);

    ntStatus = SrvGetGuestShareAccessMask(
                    pTree->pShareInfo,
                    &pResponseHeader->guestMaximalShareAccessMask);
    BAIL_ON_NT_STATUS(ntStatus);

    ntStatus = SrvGetServiceName(
                    pTree->pShareInfo,
                    &pszService);
    BAIL_ON_NT_STATUS(ntStatus);

    if (pTree->pShareInfo->service == SHARE_SERVICE_DISK_SHARE)
    {
        ntStatus = SrvGetNativeFilesystem(
                        pTree->pShareInfo,
                        &pwszNativeFileSystem);
        BAIL_ON_NT_STATUS(ntStatus);
    }

    ntStatus = MarshallTreeConnectResponseData(
                    pSmbResponse->pData,
                    pSmbResponse->bufferLen - pSmbResponse->bufferUsed,
                    pSmbResponse->bufferUsed,
                    &packetByteCount,
                    (const uint8_t*)pszService,
                    pwszNativeFileSystem);
    BAIL_ON_NT_STATUS(ntStatus);

    assert(packetByteCount <= UINT16_MAX);
    pResponseHeader->byteCount = (USHORT)packetByteCount;

    pSmbResponse->bufferUsed += packetByteCount;

    ntStatus = SMBPacketUpdateAndXOffset(pSmbResponse);
    BAIL_ON_NT_STATUS(ntStatus);

    ntStatus = SMBPacketMarshallFooter(pSmbResponse);
    BAIL_ON_NT_STATUS(ntStatus);

    *ppSmbResponse = pSmbResponse;

cleanup:

    if (pszService)
    {
        SrvFreeMemory(pszService);
    }
    if (pwszNativeFileSystem)
    {
        SrvFreeMemory(pwszNativeFileSystem);
    }

    return ntStatus;

error:

    if (pSmbResponse)
    {
        SMBPacketFree(
            pConnection->hPacketAllocator,
            pSmbResponse);
    }

    goto cleanup;
}

static
NTSTATUS
SrvGetServiceName(
    PSRV_SHARE_INFO pShareInfo,
    PSTR* ppszService
    )
{
    NTSTATUS ntStatus = 0;
    BOOLEAN bInLock = FALSE;
    PSTR pszService = NULL;

    LWIO_LOCK_RWMUTEX_SHARED(bInLock, &pShareInfo->mutex);

    ntStatus = SrvShareMapIdToServiceStringA(
                    pShareInfo->service,
                    &pszService);
    BAIL_ON_NT_STATUS(ntStatus);

    *ppszService = pszService;

cleanup:

    LWIO_UNLOCK_RWMUTEX(bInLock, &pShareInfo->mutex);

    return ntStatus;

error:

    *ppszService = NULL;

    goto cleanup;
}

static
NTSTATUS
SrvGetNativeFilesystem(
    PSRV_SHARE_INFO pShareInfo,
    PWSTR* ppwszNativeFilesystem
    )
{
    NTSTATUS ntStatus = 0;
    BOOLEAN  bInLock = FALSE;
    IO_FILE_HANDLE hFile = NULL;
    IO_FILE_NAME   fileName = {0};
    PIO_ASYNC_CONTROL_BLOCK pAsyncControlBlock = NULL;
    PVOID               pSecurityDescriptor = NULL;
    PVOID               pSecurityQOS = NULL;
    IO_STATUS_BLOCK     ioStatusBlock = {0};
    PWSTR    pwszNativeFilesystem = NULL;
    PBYTE    pVolumeInfo = NULL;
    USHORT   usBytesAllocated = 0;
    PFILE_FS_ATTRIBUTE_INFORMATION pFsAttrInfo = NULL;
    PIO_CREATE_SECURITY_CONTEXT pIoSecContext = NULL;

    LWIO_LOCK_RWMUTEX_SHARED(bInLock, &pShareInfo->mutex);

    fileName.FileName = pShareInfo->pwszPath;

    ntStatus = IoSecurityCreateSecurityContextFromUidGid(&pIoSecContext,
                             0,
                             0,
                             NULL);
    BAIL_ON_NT_STATUS(ntStatus);

    ntStatus = IoCreateFile(
                    &hFile,
                    pAsyncControlBlock,
                    &ioStatusBlock,
                    pIoSecContext,
                    &fileName,
                    pSecurityDescriptor,
                    pSecurityQOS,
                    GENERIC_READ,
                    0,
                    FILE_ATTRIBUTE_NORMAL,
                    FILE_SHARE_READ,
                    FILE_OPEN,
                    0,
                    NULL, /* EA Buffer */
                    0,    /* EA Length */
                    NULL  /* ECP List  */
                    );
    BAIL_ON_NT_STATUS(ntStatus);

    LWIO_UNLOCK_RWMUTEX(bInLock, &pShareInfo->mutex);

    usBytesAllocated = sizeof(FILE_FS_ATTRIBUTE_INFORMATION) + 256 * sizeof(wchar16_t);

    ntStatus = SrvAllocateMemory(usBytesAllocated, (PVOID*)&pVolumeInfo);
    BAIL_ON_NT_STATUS(ntStatus);

    do
    {
        ntStatus = IoQueryVolumeInformationFile(
                        hFile,
                        NULL,
                        &ioStatusBlock,
                        pVolumeInfo,
                        usBytesAllocated,
                        FileFsAttributeInformation);
        if (ntStatus == STATUS_SUCCESS)
        {
            break;
        }
        else if (ntStatus == STATUS_BUFFER_TOO_SMALL)
        {
            USHORT usNewSize = usBytesAllocated + 256 * sizeof(wchar16_t);

            ntStatus = SMBReallocMemory(
                            pVolumeInfo,
                            (PVOID*)&pVolumeInfo,
                            usNewSize);
            BAIL_ON_NT_STATUS(ntStatus);

            usBytesAllocated = usNewSize;

            continue;
        }
        BAIL_ON_NT_STATUS(ntStatus);

    } while (TRUE);

    pFsAttrInfo = (PFILE_FS_ATTRIBUTE_INFORMATION)pVolumeInfo;

    if (!pFsAttrInfo->FileSystemNameLength)
    {
        ntStatus = STATUS_INVALID_PARAMETER;
        BAIL_ON_NT_STATUS(ntStatus);
    }

    ntStatus = SMBAllocateStringW(
                    pFsAttrInfo->FileSystemName,
                    &pwszNativeFilesystem);
    BAIL_ON_NT_STATUS(ntStatus);

    *ppwszNativeFilesystem = pwszNativeFilesystem;

cleanup:

    LWIO_UNLOCK_RWMUTEX(bInLock, &pShareInfo->mutex);

    if (pVolumeInfo)
    {
        SrvFreeMemory(pVolumeInfo);
    }

    if (hFile)
    {
        IoCloseFile(hFile);
    }

    if (pIoSecContext)
    {
        IoSecurityDereferenceSecurityContext(&pIoSecContext);
    }


    return ntStatus;

error:

    *ppwszNativeFilesystem = NULL;

    goto cleanup;
}


