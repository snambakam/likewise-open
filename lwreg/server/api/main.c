/* Editor Settings: expandtabs and use 4 spaces for indentation
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 * -*- mode: c, c-basic-offset: 4 -*- */

/*
 * Copyright Likewise Software    2004-2008
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
 *        main.c
 *
 * Abstract:
 *
 *        Registry
 *
 *        Server API main
 *
 * Authors: Krishna Ganugapati (krishnag@likewisesoftware.com)
 *          Sriram Nambakam (snambakam@likewisesoftware.com)
 *          Wei Fu (wfu@likewise.com)
 */
#include "api.h"


static LWMsgDispatchSpec gMessageHandlers[] =
{
    LWMSG_DISPATCH_BLOCK(REG_Q_ENUM_ROOT_KEYSW, RegSrvIpcEnumRootKeysW),
    LWMSG_DISPATCH_BLOCK(REG_Q_ENUM_VALUEA, RegSrvIpcEnumValueA),
    LWMSG_DISPATCH_BLOCK(REG_Q_ENUM_VALUEW, RegSrvIpcEnumValueW),
    LWMSG_DISPATCH_BLOCK(REG_Q_GET_VALUEA, RegSrvIpcGetValueA),
    LWMSG_DISPATCH_BLOCK(REG_Q_GET_VALUEW, RegSrvIpcGetValueW),
    LWMSG_DISPATCH_BLOCK(REG_Q_OPEN_KEYA_EX, RegSrvIpcOpenKeyExA),
    LWMSG_DISPATCH_BLOCK(REG_Q_OPEN_KEYW_EX, RegSrvIpcOpenKeyExW),
    LWMSG_DISPATCH_BLOCK(REG_Q_QUERY_VALUEA_EX, RegSrvIpcQueryValueExA),
    LWMSG_DISPATCH_BLOCK(REG_Q_QUERY_VALUEW_EX, RegSrvIpcQueryValueExW),
    LWMSG_DISPATCH_BLOCK(REG_Q_SET_VALUEA_EX, RegSrvIpcSetValueExA),
    LWMSG_DISPATCH_BLOCK(REG_Q_SET_VALUEW_EX, RegSrvIpcSetValueExW),
    LWMSG_DISPATCH_BLOCK(REG_Q_CREATE_KEY_EX, RegSrvIpcCreateKeyEx),
    LWMSG_DISPATCH_BLOCK(REG_Q_CLOSE_KEY, RegSrvIpcCloseKey),
    LWMSG_DISPATCH_BLOCK(REG_Q_DELETE_KEY, RegSrvIpcDeleteKey),
    LWMSG_DISPATCH_BLOCK(REG_Q_DELETE_KEY_VALUE, RegSrvIpcDeleteKeyValue),
    LWMSG_DISPATCH_BLOCK(REG_Q_DELETE_TREE, RegSrvIpcDeleteTree),
    LWMSG_DISPATCH_BLOCK(REG_Q_DELETE_VALUE, RegSrvIpcDeleteValue),
    LWMSG_DISPATCH_BLOCK(REG_Q_ENUM_KEYA_EX, RegSrvIpcEnumKeyExA),
    LWMSG_DISPATCH_BLOCK(REG_Q_ENUM_KEYW_EX, RegSrvIpcEnumKeyExW),
    LWMSG_DISPATCH_BLOCK(REG_Q_QUERY_INFO_KEYA, RegSrvIpcQueryInfoKeyA),
    LWMSG_DISPATCH_BLOCK(REG_Q_QUERY_INFO_KEYW, RegSrvIpcQueryInfoKeyW),
    LWMSG_DISPATCH_BLOCK(REG_Q_QUERY_MULTIPLE_VALUES, RegSrvIpcQueryMultipleValues),
    LWMSG_DISPATCH_END
};


DWORD
RegSrvApiInit(
    VOID
    )
{
    DWORD dwError = 0;

#if defined(REG_USE_FILE)
    dwError = FileProvider_Initialize(&gpRegProvider);
    BAIL_ON_REG_ERROR(dwError);
#elif defined(REG_USE_SQLITE)
    dwError = SqliteProvider_Initialize(&gpRegProvider, ROOT_KEYS);
    BAIL_ON_REG_ERROR(dwError);
#endif

    // make sure gpRegProvider is not NULL
    if (!gpRegProvider)
    {
        dwError = LW_ERROR_INTERNAL;
        BAIL_ON_REG_ERROR(dwError);
    }

cleanup:

    return dwError;

error:

    goto cleanup;
}

static
void
RegSrvFreeProviders()
{
    //To be implemented
    return;
}

DWORD
RegSrvApiShutdown(
    VOID
    )
{
    RegSrvFreeProviders();

#if defined(REG_USE_FILE)
    FileProvider_Shutdown(gpRegProvider);
#elif defined(REG_USE_SQLITE)
    SqliteProvider_Shutdown(gpRegProvider);
#endif

    return 0;
}

LWMsgDispatchSpec*
RegSrvGetDispatchSpec(
    void
    )
{
    return gMessageHandlers;
}

