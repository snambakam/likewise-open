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

/*
 * Copyright (C) Likewise Software. All rights reserved.
 *
 * Module Name:
 *
 *        srvfinder.c
 *
 * Abstract:
 *
 *        Likewise SMB Subsystem (LWIO)
 *
 *        File and Directory object finder
 *
 * Author: Sriram Nambakam (snambakam@likewise.com)
 *
 */

#include "includes.h"

static
VOID
SrvFinderFreeRepository(
    IN PSRV_FINDER_REPOSITORY pFinderRepository
    );

static
int
SrvFinderCompareSearchSpaces(
    IN PVOID pKey1,
    IN PVOID pKey2
    );

static
VOID
SrvFinderFreeData(
    IN PVOID pData
    );

static
VOID
SrvFinderFreeSearchSpace(
    IN PSRV_SEARCH_SPACE pSearchSpace
    );

NTSTATUS
SrvFinderCreateRepository(
    OUT PHANDLE phFinderRepository
    )
{
    NTSTATUS ntStatus = 0;
    PSRV_FINDER_REPOSITORY pFinderRepository = NULL;

    ntStatus = LW_RTL_ALLOCATE(
                    &pFinderRepository,
                    SRV_FINDER_REPOSITORY,
                    sizeof(SRV_FINDER_REPOSITORY));
    BAIL_ON_NT_STATUS(ntStatus);

    pFinderRepository->refCount = 1;

    pthread_mutex_init(&pFinderRepository->mutex, NULL);
    pFinderRepository->pMutex = &pFinderRepository->mutex;

    ntStatus = LwRtlRBTreeCreate(
                    &SrvFinderCompareSearchSpaces,
                    NULL,
                    &SrvFinderFreeData,
                    &pFinderRepository->pSearchSpaceCollection);
    BAIL_ON_NT_STATUS(ntStatus);

    *phFinderRepository = pFinderRepository;

cleanup:

    return ntStatus;

error:

    *phFinderRepository = NULL;

    if (pFinderRepository)
    {
        SrvFinderFreeRepository(pFinderRepository);
    }

    goto cleanup;
}

NTSTATUS
SrvFinderCreateSearchSpace(
    IN  PIO_CREATE_SECURITY_CONTEXT pIoSecurityContext,
    IN  HANDLE         hFinderRepository,
    IN  PWSTR          pwszFilesystemPath,
    IN  PWSTR          pwszSearchPattern,
    IN  USHORT         usSearchAttrs,
    IN  ULONG          ulSearchStorageType,
    IN  SMB_INFO_LEVEL infoLevel,
    IN  BOOLEAN        bUseLongFilenames,
    OUT PHANDLE        phFinder,
    OUT PUSHORT        pusSearchId
    )
{
    NTSTATUS ntStatus = 0;
    IO_FILE_HANDLE      hFile = NULL;
    IO_STATUS_BLOCK     ioStatusBlock = {0};
    IO_FILE_NAME        fileName = {0};
    PVOID               pSecurityDescriptor = NULL;
    PVOID               pSecurityQOS = NULL;
    PSRV_FINDER_REPOSITORY pFinderRepository = NULL;
    PSRV_SEARCH_SPACE pSearchSpace = NULL;
    USHORT   usCandidateSearchId = 0;
    BOOLEAN  bFound = FALSE;
    BOOLEAN  bInLock = FALSE;

    pFinderRepository = (PSRV_FINDER_REPOSITORY)hFinderRepository;

    fileName.FileName = pwszFilesystemPath;

    ntStatus = IoCreateFile(
                    &hFile,
                    NULL,
                    &ioStatusBlock,
                    pIoSecurityContext,
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

    LWIO_LOCK_MUTEX(bInLock, &pFinderRepository->mutex);

    usCandidateSearchId = pFinderRepository->usNextSearchId;

    do
    {
        PSRV_SEARCH_SPACE pSearchSpace = NULL;

        if (!usCandidateSearchId || (usCandidateSearchId == UINT16_MAX))
        {
            usCandidateSearchId++;
        }

        ntStatus = LwRtlRBTreeFind(
                        pFinderRepository->pSearchSpaceCollection,
                        &usCandidateSearchId,
                        (PVOID*)&pSearchSpace);
        if (ntStatus == STATUS_NOT_FOUND)
        {
            ntStatus = 0;
            bFound = TRUE;
        }
        BAIL_ON_NT_STATUS(ntStatus);

    } while ((usCandidateSearchId != pFinderRepository->usNextSearchId) && !bFound);

    if (!bFound)
    {
        ntStatus = STATUS_TOO_MANY_OPENED_FILES;
        BAIL_ON_NT_STATUS(ntStatus);
    }

    ntStatus = LW_RTL_ALLOCATE(
                    &pSearchSpace,
                    SRV_SEARCH_SPACE,
                    sizeof(SRV_SEARCH_SPACE));
    BAIL_ON_NT_STATUS(ntStatus);

    pSearchSpace->refCount = 1;

    pthread_mutex_init(&pSearchSpace->mutex, NULL);
    pSearchSpace->pMutex = &pSearchSpace->mutex;

    pSearchSpace->usSearchId = usCandidateSearchId;

    ntStatus = LwRtlRBTreeAdd(
                    pFinderRepository->pSearchSpaceCollection,
                    &pSearchSpace->usSearchId,
                    pSearchSpace);
    BAIL_ON_NT_STATUS(ntStatus);

    pSearchSpace->hFile = hFile;
    hFile = NULL;
    pSearchSpace->infoLevel = infoLevel;
    pSearchSpace->usSearchAttrs = usSearchAttrs;
    pSearchSpace->ulSearchStorageType = ulSearchStorageType;
    pSearchSpace->bUseLongFilenames = bUseLongFilenames;

    ntStatus = SMBAllocateStringW(
                    pwszSearchPattern,
                    &pSearchSpace->pwszSearchPattern);
    BAIL_ON_NT_STATUS(ntStatus);

    InterlockedIncrement(&pSearchSpace->refCount);

    pFinderRepository->usNextSearchId = usCandidateSearchId + 1;

    *phFinder = pSearchSpace;
    *pusSearchId = usCandidateSearchId;

cleanup:

    LWIO_UNLOCK_MUTEX(bInLock, &pFinderRepository->mutex);

    return ntStatus;

error:

    *phFinder = NULL;
    *pusSearchId = 0;

    if (pSearchSpace)
    {
        SrvFinderReleaseSearchSpace(pSearchSpace);
    }

    if (hFile)
    {
        IoCloseFile(hFile);
    }

    goto cleanup;
}

NTSTATUS
SrvFinderGetSearchSpace(
    IN  HANDLE  hFinderRepository,
    IN  USHORT  usSearchId,
    OUT PHANDLE phFinder
    )
{
    NTSTATUS ntStatus = 0;
    PSRV_FINDER_REPOSITORY pFinderRepository = NULL;
    PSRV_SEARCH_SPACE pSearchSpace = NULL;
    BOOLEAN bInLock = FALSE;

    pFinderRepository = (PSRV_FINDER_REPOSITORY)hFinderRepository;

    LWIO_LOCK_MUTEX(bInLock, &pFinderRepository->mutex);

    ntStatus = LwRtlRBTreeFind(
                    pFinderRepository->pSearchSpaceCollection,
                    &usSearchId,
                    (PVOID*)&pSearchSpace);
    BAIL_ON_NT_STATUS(ntStatus);

    InterlockedIncrement(&pSearchSpace->refCount);

    *phFinder = pSearchSpace;

cleanup:

    LWIO_UNLOCK_MUTEX(bInLock, &pFinderRepository->mutex);

    return ntStatus;

error:

    *phFinder = NULL;

    goto cleanup;
}

VOID
SrvFinderReleaseSearchSpace(
    IN HANDLE hFinder
    )
{
    PSRV_SEARCH_SPACE pSearchSpace = (PSRV_SEARCH_SPACE)hFinder;

    if (InterlockedDecrement(&pSearchSpace->refCount) == 0)
    {
        SrvFinderFreeSearchSpace(pSearchSpace);
    }
}

NTSTATUS
SrvFinderCloseSearchSpace(
    IN HANDLE hFinderRepository,
    IN USHORT usSearchId
    )
{
    NTSTATUS ntStatus = 0;
    PSRV_FINDER_REPOSITORY pFinderRepository = NULL;
    BOOLEAN bInLock = FALSE;

    pFinderRepository = (PSRV_FINDER_REPOSITORY)hFinderRepository;

    LWIO_LOCK_MUTEX(bInLock, &pFinderRepository->mutex);

    ntStatus = LwRtlRBTreeRemove(
                    pFinderRepository->pSearchSpaceCollection,
                    &usSearchId);
    BAIL_ON_NT_STATUS(ntStatus);

cleanup:

    LWIO_UNLOCK_MUTEX(bInLock, &pFinderRepository->mutex);

    return ntStatus;

error:

    goto cleanup;
}

VOID
SrvFinderCloseRepository(
    IN HANDLE hFinderRepository
    )
{
    PSRV_FINDER_REPOSITORY pFinderRepository = NULL;

    pFinderRepository = (PSRV_FINDER_REPOSITORY)hFinderRepository;

    if (InterlockedDecrement(&pFinderRepository->refCount) == 0)
    {
        SrvFinderFreeRepository(pFinderRepository);
    }
}

static
VOID
SrvFinderFreeRepository(
    IN PSRV_FINDER_REPOSITORY pFinderRepository
    )
{
    if (pFinderRepository->pSearchSpaceCollection)
    {
        LwRtlRBTreeFree(pFinderRepository->pSearchSpaceCollection);
    }

    if (pFinderRepository->pMutex)
    {
        pthread_mutex_destroy(&pFinderRepository->mutex);
    }

    LwRtlMemoryFree(pFinderRepository);
}

static
int
SrvFinderCompareSearchSpaces(
    IN PVOID pKey1,
    IN PVOID pKey2
    )
{
    USHORT usKey1 = *((PUSHORT)pKey1);
    USHORT usKey2 = *((PUSHORT)pKey2);

    if (usKey1 > usKey2)
    {
        return 1;
    }
    else if (usKey1 < usKey2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static
VOID
SrvFinderFreeData(
    IN PVOID pData
    )
{
    PSRV_SEARCH_SPACE pSearchSpace = (PSRV_SEARCH_SPACE)pData;

    if (InterlockedDecrement(&pSearchSpace->refCount) == 0)
    {
        SrvFinderFreeSearchSpace(pSearchSpace);
    }
}

static
VOID
SrvFinderFreeSearchSpace(
    IN PSRV_SEARCH_SPACE pSearchSpace
    )
{
    if (pSearchSpace->pMutex)
    {
        pthread_mutex_destroy(&pSearchSpace->mutex);
    }

    if (pSearchSpace->hFile)
    {
        IoCloseFile(pSearchSpace->hFile);
    }

    if (pSearchSpace->pFileInfo)
    {
        LwRtlMemoryFree(pSearchSpace->pFileInfo);
    }

    if (pSearchSpace->pwszSearchPattern)
    {
        SMBFreeMemory(pSearchSpace->pwszSearchPattern);

    }

    LwRtlMemoryFree(pSearchSpace);
}
