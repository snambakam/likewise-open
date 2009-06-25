/* Editor Settings: expandtabs and use 4 spaces for indentation
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 */

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
 *        session_setup.c
 *
 * Abstract:
 *
 *        Likewise IO (LWIO) - SRV
 *
 *        Protocols API - SMBV2
 *
 *        Session Setup
 *
 * Authors: Sriram Nambakam (snambakam@likewise.com)
 *
 */
#include "includes.h"

NTSTATUS
SrvProcessSessionSetup_SMB_V2(
    PLWIO_SRV_CONNECTION pConnection,
    PSMB_PACKET          pSmbRequest,
    PSMB_PACKET*         ppSmbResponse
    )
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PSMB2_SESSION_SETUP_REQUEST_HEADER pSessionSetupHeader = NULL;// Do not free
    PBYTE       pSecurityBlob = NULL; // Do not free
    ULONG       ulSecurityBlobLen = 0;
    PBYTE       pReplySecurityBlob = NULL;
    ULONG       ulReplySecurityBlobLength = 0;
    UNICODE_STRING uniUsername = {0};
    PLWIO_SRV_SESSION_2 pSession = NULL;
    PSMB_PACKET pSmbResponse = NULL;

    ntStatus = SMB2UnmarshallSessionSetup(
                    pSmbRequest,
                    &pSessionSetupHeader,
                    &pSecurityBlob,
                    &ulSecurityBlobLen);
    BAIL_ON_NT_STATUS(ntStatus);

    ntStatus = SrvGssNegotiate(
                    pConnection->hGssContext,
                    pConnection->hGssNegotiate,
                    pSecurityBlob,
                    ulSecurityBlobLen,
                    &pReplySecurityBlob,
                    &ulReplySecurityBlobLength);
    BAIL_ON_NT_STATUS(ntStatus);

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

    ntStatus = SMB2MarshalHeader(
                    pSmbResponse,
                    COM2_SESSION_SETUP,
                    0,
                    9,
                    pSmbRequest->pSMB2Header->ulPid,
                    pSmbRequest->pSMB2Header->ullCommandSequence,
                    pSmbRequest->pSMB2Header->ulTid,
                    0LL,
                    STATUS_SUCCESS,
                    TRUE,
                    TRUE);
    BAIL_ON_NT_STATUS(ntStatus);

    if (!SrvGssNegotiateIsComplete(pConnection->hGssContext,
                                   pConnection->hGssNegotiate))
    {
        pSmbResponse->pSMB2Header->error = STATUS_MORE_PROCESSING_REQUIRED;
    }
    else
    {
        ntStatus = SrvConnection2CreateSession(
                            pConnection,
                            &pSession);
        BAIL_ON_NT_STATUS(ntStatus);

        if (!pConnection->pSessionKey)
        {
             ntStatus = SrvGssGetSessionDetails(
                             pConnection->hGssContext,
                             pConnection->hGssNegotiate,
                             &pConnection->pSessionKey,
                             &pConnection->ulSessionKeyLength,
                             &pSession->pszClientPrincipalName);
        }
        else
        {
             ntStatus = SrvGssGetSessionDetails(
                             pConnection->hGssContext,
                             pConnection->hGssNegotiate,
                             NULL,
                             NULL,
                             &pSession->pszClientPrincipalName);
        }
        BAIL_ON_NT_STATUS(ntStatus);

        /* Generate and store the IoSecurityContext */

        ntStatus = RtlUnicodeStringAllocateFromCString(
                       &uniUsername,
                       pSession->pszClientPrincipalName);
        BAIL_ON_NT_STATUS(ntStatus);

        ntStatus = IoSecurityCreateSecurityContextFromUsername(
                       &pSession->pIoSecurityContext,
                       &uniUsername);
        BAIL_ON_NT_STATUS(ntStatus);

        pSmbResponse->pSMB2Header->ullSessionId = pSession->ullUid;

        SrvConnectionSetState(pConnection, LWIO_SRV_CONN_STATE_READY);
    }

    ntStatus = SMB2MarshalSessionSetup(
                    pSmbResponse,
                    0,
                    pReplySecurityBlob,
                    ulReplySecurityBlobLength);
    BAIL_ON_NT_STATUS(ntStatus);

    ntStatus = SMB2MarshalFooter(pSmbResponse);
    BAIL_ON_NT_STATUS(ntStatus);

    *ppSmbResponse = pSmbResponse;

cleanup:

    if (pSession)
    {
        SrvSession2Release(pSession);
    }

    RtlUnicodeStringFree(&uniUsername);

    SRV_SAFE_FREE_MEMORY(pReplySecurityBlob);

    return ntStatus;

error:

    *ppSmbResponse = NULL;

    if (pSmbResponse)
    {
        SMBPacketFree(pConnection->hPacketAllocator, pSmbResponse);
    }

    goto cleanup;
}
