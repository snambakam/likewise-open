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

#include "includes.h"


NTSTATUS
SamrGetMembersInAlias(
    handle_t b,
    PolicyHandle *alias_h,
    PSID** sids,
    uint32 *count
    )
{
    NTSTATUS status = STATUS_SUCCESS;
    SidArray sid_array = {0};
    PSID* out_sids = NULL;

    BAIL_ON_INVALID_PTR(b);
    BAIL_ON_INVALID_PTR(alias_h);
    BAIL_ON_INVALID_PTR(sids);
    BAIL_ON_INVALID_PTR(count);

    DCERPC_CALL(_SamrGetMembersInAlias(b, alias_h, &sid_array));

    BAIL_ON_NTSTATUS_ERROR(status);

    status = SamrAllocateSids(&out_sids, &sid_array);
    BAIL_ON_NTSTATUS_ERROR(status);

    *count = sid_array.num_sids;
    *sids  = out_sids;

cleanup:
    SamrCleanStubSidArray(&sid_array);

    return status;

error:
    if (out_sids) {
        SamrFreeMemory((void*)out_sids);
    }

    *sids  = NULL;
    *count = 0;
    goto cleanup;
}


/*
local variables:
mode: c
c-basic-offset: 4
indent-tabs-mode: nil
tab-width: 4
end:
*/
