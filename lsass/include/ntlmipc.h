/* Editor Settings: expandtabs and use 4 spaces for indentation
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 */

/*
 * Copyright Likewise Software    2004-2008
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the license, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 * General Public License for more details.  You should have received a copy
 * of the GNU Lesser General Public License along with this program.  If
 * not, see <http://www.gnu.org/licenses/>.
 *
 * LIKEWISE SOFTWARE MAKES THIS SOFTWARE AVAILABLE UNDER OTHER LICENSING
 * TERMS AS WELL.  IF YOU HAVE ENTERED INTO A SEPARATE LICENSE AGREEMENT
 * WITH LIKEWISE SOFTWARE, THEN YOU MAY ELECT TO USE THE SOFTWARE UNDER THE
 * TERMS OF THAT SOFTWARE LICENSE AGREEMENT INSTEAD OF THE TERMS OF THE GNU
 * LESSER GENERAL PUBLIC LICENSE, NOTWITHSTANDING THE ABOVE NOTICE.  IF YOU
 * HAVE QUESTIONS, OR WISH TO REQUEST A COPY OF THE ALTERNATE LICENSING
 * TERMS OFFERED BY LIKEWISE SOFTWARE, PLEASE CONTACT LIKEWISE SOFTWARE AT
 * license@likewisesoftware.com
 */



/*
 * Copyright (C) Likewise Software. All rights reserved.
 *
 * Module Name:
 *
 *        lsaipc.h
 *
 * Abstract:
 *
 *        Likewise Security and Authentication Subsystem (LSASS) Interprocess Communication
 *
 * Authors: Krishna Ganugapati (krishnag@likewisesoftware.com)
 *          Sriram Nambakam (snambakam@likewisesoftware.com)
 *
 */
#ifndef __NTLMIPC_H__
#define __NTLMIPC_H__

#include <lwmsg/lwmsg.h>

#define NTLM_CLIENT_PATH_FORMAT "/var/tmp/.ntlmclient_%05ld"
#define NTLM_SERVER_FILENAME    ".ntlmsd"

typedef enum __NTLM_IPC_TAG
{
    NTLM_Q_ACCEPT_SEC_CTXT,
    NTLM_R_ACCEPT_SEC_CTXT_SUCCESS,
    NTLM_R_ACCEPT_SEC_CTXT_FAILURE,
    NTLM_Q_ACQUIRE_CREDS,
    NTLM_R_ACQUIRE_CREDS_SUCCESS,
    NTLM_R_ACQUIRE_CREDS_FAILURE,
    NTLM_Q_DECRYPT_MSG,
    NTLM_R_DECRYPT_MSG_SUCCESS,
    NTLM_R_DECRYPT_MSG_FAILURE,
    NTLM_Q_DELETE_SEC_CTXT,
    NTLM_R_DELETE_SEC_CTXT_SUCCESS,
    NTLM_R_DELETE_SEC_CTXT_FAILURE,
    NTLM_Q_ENCRYPT_MSG,
    NTLM_R_ENCRYPT_MSG_SUCCESS,
    NTLM_R_ENCRYPT_MSG_FAILURE,
    NTLM_Q_EXPORT_SEC_CTXT,
    NTLM_R_EXPORT_SEC_CTXT_SUCCESS,
    NTLM_R_EXPORT_SEC_CTXT_FAILURE,
    NTLM_Q_FREE_CREDS,
    NTLM_R_FREE_CREDS_SUCCESS,
    NTLM_R_FREE_CREDS_FAILURE,
    NTLM_Q_IMPORT_SEC_CTXT,
    NTLM_R_IMPORT_SEC_CTXT_SUCCESS,
    NTLM_R_IMPORT_SEC_CTXT_FAILURE,
    NTLM_Q_INIT_SEC_CTXT,
    NTLM_R_INIT_SEC_CTXT_SUCCESS,
    NTLM_R_INIT_SEC_CTXT_FAILURE,
    NTLM_Q_MAKE_SIGN,
    NTLM_R_MAKE_SIGN_SUCCESS,
    NTLM_R_MAKE_SIGN_FAILURE,
    NTLM_Q_QUERY_CREDS,
    NTLM_R_QUERY_CREDS_SUCCESS,
    NTLM_R_QUERY_CREDS_FAILURE,
    NTLM_Q_QUERY_CTXT,
    NTLM_R_QUERY_CTXT_SUCCESS,
    NTLM_R_QUERY_CTXT_FAILURE,
    NTLM_Q_VERIFY_SIGN,
    NTLM_R_VERIFY_SIGN_SUCCESS,
    NTLM_R_VERIFY_SIGN_FAILURE
} NTLM_IPC_TAG;

/******************************************************************************/

typedef struct __NTLM_IPC_ERROR
{
    DWORD dwError;
} NTLM_IPC_ERROR, *PNTLM_IPC_ERROR;

/******************************************************************************/

typedef struct __NTLM_IPC_ACCEPT_SEC_CTXT_REQ
{
    PCredHandle phCredential;
    PCtxtHandle phContext;
    PSecBufferDesc pInput;
    DWORD fContextReq;
    DWORD TargetDataRep;
    PCtxtHandle phNewContext;
    PSecBufferDesc pOutput;
} NTLM_IPC_ACCEPT_SEC_CTXT_REQ, *PNTLM_IPC_ACCEPT_SEC_CTXT_REQ;

typedef struct __NTLM_IPC_ACCEPT_SEC_CTXT_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_ACCEPT_SEC_CTXT_RESPONSE, *PNTLM_IPC_ACCEPT_SEC_CTXT_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_ACQUIRE_CREDS_REQ
{
    SEC_CHAR *pszPrincipal;
    SEC_CHAR *pszPackage;
    DWORD fCredentialUse;
    PLUID pvLogonID;
    PVOID pAuthData;
    // NOT USED BY NTLM - SEC_GET_KEY_FN pGetKeyFn;
    // NOT USED BY NTLM - PVOID pvGetKeyArgument;
} NTLM_IPC_ACQUIRE_CREDS_REQ, *PNTLM_IPC_ACQUIRE_CREDS_REQ;

typedef struct __NTLM_IPC_ACQUIRE_CREDS_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_ACQUIRE_CREDS_RESPONSE, *PNTLM_IPC_ACQUIRE_CREDS_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_DECRYPT_MSG_REQ
{
    PCtxtHandle phContext;
    PSecBufferDesc pMessage;
    DWORD MessageSeqNo;
} NTLM_IPC_DECRYPT_MSG_REQ, *PNTLM_IPC_DECRYPT_MSG_REQ;

typedef struct __NTLM_IPC_DECRYPT_MSG_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_DECRYPT_MSG_RESPONSE, *PNTLM_IPC_DECRYPT_MSG_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_DELETE_SEC_CTXT_REQ
{
    PCtxtHandle phContext;
} NTLM_IPC_DELETE_SEC_CTXT_REQ, *PNTLM_IPC_DELETE_SEC_CTXT_REQ;

typedef struct __NTLM_IPC_DELETE_SEC_CTXT_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_DELETE_SEC_CTXT_RESPONSE, *PNTLM_IPC_DELETE_SEC_CTXT_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_ENCRYPT_MSG_REQ
{
    PCtxtHandle phContext;
    BOOL bEncrypt;
    PSecBufferDesc pMessage;
    DWORD MessageSeqNo;
} NTLM_IPC_ENCRYPT_MSG_REQ, *PNTLM_IPC_ENCRYPT_MSG_REQ;

typedef struct __NTLM_IPC_ENCRYPT_MSG_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_ENCRYPT_MSG_RESPONSE, *PNTLM_IPC_ENCRYPT_MSG_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_EXPORT_SEC_CTXT_REQ
{
    PCtxtHandle phContext;
    DWORD fFlags;
    PSecBuffer pPackedContext;
    HANDLE *pToken;
} NTLM_IPC_EXPORT_SEC_CTXT_REQ, *PNTLM_IPC_EXPORT_SEC_CTXT_REQ;

typedef struct __NTLM_IPC_EXPORT_SEC_CTXT_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_EXPORT_SEC_CTXT_RESPONSE, *PNTLM_IPC_EXPORT_SEC_CTXT_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_FREE_CREDS_REQ
{
    PCredHandle phCredential;
} NTLM_IPC_FREE_CREDS_REQ, *PNTLM_IPC_FREE_CREDS_REQ;

typedef struct __NTLM_IPC_FREE_CREDS_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_FREE_CREDS_RESPONSE, *PNTLM_IPC_FREE_CREDS_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_IMPORT_SEC_CTXT_REQ
{
    PSECURITY_STRING *pszPackage;
    PSecBuffer pPackedContext;
    HANDLE pToken;
    PCtxtHandle phContext;
} NTLM_IPC_IMPORT_SEC_CTXT_REQ, *PNTLM_IPC_IMPORT_SEC_CTXT_REQ;

typedef struct __NTLM_IPC_IMPORT_SEC_CTXT_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_IMPORT_SEC_CTXT_RESPONSE, *PNTLM_IPC_IMPORT_SEC_CTXT_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_INIT_SEC_CTXT_REQ
{
    PCredHandle phCredential;
    PCtxtHandle phContext;
    SEC_CHAR * pszTargetName;
    DWORD fContextReq;
    DWORD Reserved1;
    DWORD TargetDataRep;
    PSecBufferDesc pInput;
    DWORD Reserved2;
    PCtxtHandle phNewContext;
    PSecBufferDesc pOutput;
} NTLM_IPC_INIT_SEC_CTXT_REQ, *PNTLM_IPC_INIT_SEC_CTXT_REQ;

typedef struct __NTLM_IPC_INIT_SEC_CTXT_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_INIT_SEC_CTXT_RESPONSE, *PNTLM_IPC_INIT_SEC_CTXT_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_MAKE_SIGN_REQ
{
    PCtxtHandle phContext;
    BOOL bEncrypt;
    PSecBufferDesc pMessage;
    DWORD MessageSeqNo;
} NTLM_IPC_MAKE_SIGN_REQ, *PNTLM_IPC_MAKE_SIGN_REQ;

typedef struct __NTLM_IPC_MAKE_SIGN_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_MAKE_SIGN_RESPONSE, *PNTLM_IPC_MAKE_SIGN_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_QUERY_CREDS_REQ
{
    PCredHandle phCredential;
    DWORD ulAttribute;
} NTLM_IPC_QUERY_CREDS_REQ, *PNTLM_IPC_QUERY_CREDS_REQ;

typedef struct __NTLM_IPC_QUERY_CREDS_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_QUERY_CREDS_RESPONSE, *PNTLM_IPC_QUERY_CREDS_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_QUERY_CTXT_REQ
{
    PCtxtHandle phContext;
    DWORD ulAttribute;
} NTLM_IPC_QUERY_CTXT_REQ, *PNTLM_IPC_QUERY_CTXT_REQ;

typedef struct __NTLM_IPC_QUERY_CTXT_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_QUERY_CTXT_RESPONSE, *PNTLM_IPC_QUERY_CTXT_RESPONSE;

/******************************************************************************/

typedef struct __NTLM_IPC_VERIFY_SIGN_REQ
{
    PCtxtHandle phContext;
    PSecBufferDesc pMessage;
    DWORD MessageSeqNo;
} NTLM_IPC_VERIFY_SIGN_REQ, *PNTLM_IPC_VERIFY_SIGN_REQ;

typedef struct __NTLM_IPC_VERIFY_SIGN_RESPONSE
{
    DWORD dwError;
} NTLM_IPC_VERIFY_SIGN_RESPONSE, *PNTLM_IPC_VERIFY_SIGN_RESPONSE;

/******************************************************************************/

#define NTLM_MAP_LWMSG_ERROR(_e_) (NtlmMapLwmsgStatus(_e_))
#define MAP_NTLM_ERROR_IPC(_e_) ((_e_) ? LWMSG_STATUS_ERROR : LWMSG_STATUS_SUCCESS)

LWMsgProtocolSpec*
NtlmIpcGetProtocolSpec(
    void
    );

DWORD
NtlmOpenServer(
    PHANDLE phConnection
    );

DWORD
NtlmCloseServer(
    HANDLE hConnection
    );

DWORD
NtlmWriteData(
    DWORD dwFd,
    PSTR  pszBuf,
    DWORD dwLen);

DWORD
NtlmReadData(
    DWORD  dwFd,
    PSTR   pszBuf,
    DWORD  dwBytesToRead,
    PDWORD pdwBytesRead);

DWORD
NtlmMapLwmsgStatus(
    LWMsgStatus status
    );

DWORD
NtlmSrvIpcCreateError(
    DWORD dwErrorCode,
    PNTLM_IPC_ERROR* ppError
    );

LWMsgStatus
NtlmSrvIpcAcceptSecurityContext(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcAcquireCredentialsHandle(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcDecryptMessage(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcDeleteSecurityContext(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcEncryptMessage(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcExportSecurityContext(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcFreeCredentialsHandle(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcImportSecurityContext(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcInitializeSecurityContext(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcMakeSignature(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcQueryCredentialsAttributes(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcQueryContextAttributes(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

LWMsgStatus
NtlmSrvIpcVerifySignature(
    LWMsgAssoc* assoc,
    const LWMsgMessage* pRequest,
    LWMsgMessage* pResponse,
    void* data
    );

#endif /*__NTLMIPC_H__*/


/*
local variables:
mode: c
c-basic-offset: 4
indent-tabs-mode: nil
tab-width: 4
end:
*/
