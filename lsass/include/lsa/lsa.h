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
 *        lsa.h
 *
 * Abstract:
 *
 *        Likewise Security and Authentication Subsystem (LSASS)
 *
 *        Public Client API
 *
 * Authors: Krishna Ganugapati (krishnag@likewisesoftware.com)
 *          Sriram Nambakam (snambakam@likewisesoftware.com)
 */
#ifndef __LSA_H__
#define __LSA_H__

/**
 * @file lsa.h
 * @brief LSASS Public Client API
 */

/**
 * @defgroup public Public API
 *
 */

/**
 * @defgroup connection Connections
 * @ingroup public
 */

/**
 * @defgroup user Users
 * @ingroup public
 */

/**
 * @defgroup group Groups
 * @ingroup public
 */

/**
 * @defgroup artifacts Artifacts
 * @ingroup public
 */

/**
 * @defgroup utility Utility
 * @ingroup public
 */

#include <lw/types.h>
#include <lw/attrs.h>

#include "lwerror.h"


typedef LW_DWORD LSA_DS_FLAGS, *PLSA_DS_FLAGS;

#define LSA_DS_DNS_CONTROLLER_FLAG  0x20000000
#define LSA_DS_DNS_DOMAIN_FLAG      0x40000000
#define LSA_DS_DNS_FOREST_FLAG      0x80000000
#define LSA_DS_DS_FLAG              0x00000010
#define LSA_DS_GC_FLAG              0x00000004
#define LSA_DS_KDC_FLAG             0x00000020
#define LSA_DS_PDC_FLAG             0x00000001
#define LSA_DS_TIMESERV_FLAG        0x00000040
#define LSA_DS_WRITABLE_FLAG        0x00000100

typedef LW_DWORD LSA_DM_DOMAIN_FLAGS, *PLSA_DM_DOMAIN_FLAGS;

#define LSA_DM_DOMAIN_FLAG_PRIMARY               0x00000001
#define LSA_DM_DOMAIN_FLAG_OFFLINE               0x00000002
#define LSA_DM_DOMAIN_FLAG_FORCE_OFFLINE         0x00000004
#define LSA_DM_DOMAIN_FLAG_TRANSITIVE_1WAY_CHILD 0x00000008
#define LSA_DM_DOMAIN_FLAG_FOREST_ROOT           0x00000010
#define LSA_DM_DOMAIN_FLAG_GC_OFFLINE            0x00000020


#define LSA_DM_DOMAIN_FLAGS_VALID_MASK \
    ( \
        LSA_DM_DOMAIN_FLAG_PRIMARY | \
        LSA_DM_DOMAIN_FLAG_OFFLINE | \
        LSA_DM_DOMAIN_FLAG_FORCE_OFFLINE | \
        LSA_DM_DOMAIN_FLAG_TRANSITIVE_1WAY_CHILD | \
        LSA_DM_DOMAIN_FLAG_FOREST_ROOT | \
        LSA_DM_DOMAIN_FLAG_GC_OFFLINE | \
        0 \
    )

typedef LW_DWORD LSA_DM_STATE_FLAGS, *PLSA_DM_STATE_FLAGS;

/// Controls whether to enable offline reporting.
/// Offline state is always tracked internally,
/// but this controls whether to honor that state.
#define LSA_DM_STATE_FLAG_OFFLINE_ENABLED        0x00000001
/// Whether forced globally offline (by user).
#define LSA_DM_STATE_FLAG_FORCE_OFFLINE          0x00000002
/// Whether globally offline due to media sense.
#define LSA_DM_STATE_FLAG_MEDIA_SENSE_OFFLINE    0x00000004

#define LSA_DM_STATE_FLAGS_VALID_MASK \
    ( \
        LSA_DM_STATE_FLAG_OFFLINE_ENABLED | \
        LSA_DM_STATE_FLAG_FORCE_OFFLINE | \
        LSA_DM_STATE_FLAG_MEDIA_SENSE_OFFLINE | \
        0 \
    )

typedef LW_DWORD LSA_TRUST_TYPE, *PLSA_TRUST_TYPE;

#define LSA_TRUST_TYPE_DOWNLEVEL            0x00000001
#define LSA_TRUST_TYPE_UPLEVEL              0x00000002
#define LSA_TRUST_TYPE_MIT                  0x00000003
#define LSA_TRUST_TYPE_DCE                  0x00000004

typedef LW_DWORD LSA_TRUST_ATTRIBUTE, *PLSA_TRUST_ATTRIBUTE;

#define LSA_TRUST_ATTRIBUTE_NON_TRANSITIVE     0x00000001
#define LSA_TRUST_ATTRIBUTE_UPLEVEL_ONLY       0x00000002
#define LSA_TRUST_ATTRIBUTE_FILTER_SIDS        0x00000004
#define LSA_TRUST_ATTRIBUTE_FOREST_TRANSITIVE  0x00000008
#define LSA_TRUST_ATTRIBUTE_CROSS_ORGANIZATION 0x00000010
#define LSA_TRUST_ATTRIBUTE_WITHIN_FOREST      0x00000020

typedef LW_DWORD LSA_TRUST_FLAG, *PLSA_TRUST_FLAG;

#define LSA_TRUST_FLAG_IN_FOREST    0x00000001
#define LSA_TRUST_FLAG_OUTBOUND     0x00000002
#define LSA_TRUST_FLAG_TREEROOT     0x00000004
#define LSA_TRUST_FLAG_PRIMARY      0x00000008
#define LSA_TRUST_FLAG_NATIVE       0x00000010
#define LSA_TRUST_FLAG_INBOUND      0x00000020

typedef LW_DWORD LSA_TRUST_DIRECTION;

#define LSA_TRUST_DIRECTION_UNKNOWN  0x00000000
#define LSA_TRUST_DIRECTION_ZERO_WAY 0x00000001
#define LSA_TRUST_DIRECTION_ONE_WAY  0x00000002
#define LSA_TRUST_DIRECTION_TWO_WAY  0x00000003
#define LSA_TRUST_DIRECTION_SELF     0x00000004

typedef LW_DWORD LSA_TRUST_MODE;

#define LSA_TRUST_MODE_UNKNOWN       0x00000000
#define LSA_TRUST_MODE_EXTERNAL      0x00000001
#define LSA_TRUST_MODE_MY_FOREST     0x00000002
#define LSA_TRUST_MODE_OTHER_FOREST  0x00000003

#define LSA_NIS_MAP_NAME_NETGROUPS  "netgroup"
#define LSA_NIS_MAP_NAME_SERVICES   "services"
#define LSA_NIS_MAP_NAME_AUTOMOUNTS "automounts"

typedef LW_DWORD LSA_NIS_MAP_QUERY_FLAGS;

#define LSA_NIS_MAP_QUERY_KEYS       0x00000001
#define LSA_NIS_MAP_QUERY_VALUES     0x00000002
#define LSA_NIS_MAP_QUERY_ALL        (LSA_NIS_MAP_QUERY_KEYS | LSA_NIS_MAP_QUERY_VALUES)

typedef LW_DWORD LSA_FIND_FLAGS, *PLSA_FIND_FLAGS;

#define LSA_FIND_FLAGS_NSS 0x00000001

/*
 * Tracing support
 */
#define LSA_TRACE_FLAG_USER_GROUP_QUERIES        1
#define LSA_TRACE_FLAG_AUTHENTICATION            2
#define LSA_TRACE_FLAG_USER_GROUP_ADMINISTRATION 3
#define LSA_TRACE_FLAG_SENTINEL                  4

typedef struct __LSA_TRACE_INFO
{
    LW_DWORD dwTraceFlag;
    LW_BOOLEAN bStatus;
} LSA_TRACE_INFO, *PLSA_TRACE_INFO;

typedef struct __LSA_TRACE_INFO_LIST
{
    LW_DWORD dwNumFlags;
    PLSA_TRACE_INFO pTraceInfoArray;
} LSA_TRACE_INFO_LIST, *PLSA_TRACE_INFO_LIST;


/*
 * Logging
 */
typedef enum
{
    LSA_LOG_LEVEL_ALWAYS = 0,
    LSA_LOG_LEVEL_ERROR,
    LSA_LOG_LEVEL_WARNING,
    LSA_LOG_LEVEL_INFO,
    LSA_LOG_LEVEL_VERBOSE,
    LSA_LOG_LEVEL_DEBUG,
    LSA_LOG_LEVEL_TRACE
} LsaLogLevel;

typedef enum
{
    LSA_LOG_TARGET_DISABLED = 0,
    LSA_LOG_TARGET_CONSOLE,
    LSA_LOG_TARGET_FILE,
    LSA_LOG_TARGET_SYSLOG
} LsaLogTarget;

typedef LW_VOID (*PFN_LSA_LOG_MESSAGE)(
    LW_HANDLE hLog,
    LsaLogLevel logLevel,
    LW_PCSTR pszFormat,
    va_list msgList
    );

typedef struct __LSA_LOG_INFO {
    LsaLogLevel maxAllowedLogLevel;
    LsaLogTarget logTarget;
    LW_PSTR pszPath;
} LSA_LOG_INFO, *PLSA_LOG_INFO;

/**
 * @ingroup user
 * @brief User info structure -- level 0
 *
 * Describes the basic attributes of a user,
 * particularly those which are present in the
 * classic UNIX passwd structure.
 */
typedef struct __LSA_USER_INFO_0
{
    /** @brief User ID */
    uid_t uid;
    /** @brief Primary group ID */
    gid_t gid;
    /** @brief Username (alias) */
    LW_PSTR pszName;
    /** @brief Password (may be NULL) */
    LW_PSTR pszPasswd;
    /** @brief Comment */
    LW_PSTR pszGecos;
    /** @brief Login shell path */
    LW_PSTR pszShell;
    /** @brief Home directory path */
    LW_PSTR pszHomedir;
    /** @brief Windows SID in string form (may be NULL) */
    LW_PSTR pszSid;
} LSA_USER_INFO_0, *PLSA_USER_INFO_0;

/**
 * @ingroup user
 * @brief User info structure -- level 1
 *
 * Describes everything about a user included in #__LSA_USER_INFO_0
 * in addition to several attributes which tend to be applicable
 * only in Windows network environments.
 */
typedef struct __LSA_USER_INFO_1
{
#ifndef DOXYGEN
    union
    {
        struct
        {
#endif
            uid_t uid;
            gid_t gid;
            LW_PSTR pszName;
            LW_PSTR pszPasswd;
            LW_PSTR pszGecos;
            LW_PSTR pszShell;
            LW_PSTR pszHomedir;
            LW_PSTR pszSid;
#ifndef DOXYGEN
        };
        LSA_USER_INFO_0 info0;
    };
#endif
    /** @brief User object DN */
    LW_PSTR pszDN;
    /** @brief User's Kerberos UPN */
    LW_PSTR pszUPN;
    /** @brief Whether the UPN is explicit or implicit */
    LW_DWORD bIsGeneratedUPN;
    /** @brief Whether the user is from a local account database */
    LW_DWORD bIsLocalUser;
    /** @brief LM hash of the user's password */
    LW_PBYTE pLMHash;
    /** @brief Length of the LM hash */
    LW_DWORD dwLMHashLen;
    /** @brief NT hash of the user's password */
    LW_PBYTE pNTHash;
    /** @brief Length of the NT hash */
    LW_DWORD dwNTHashLen;
} LSA_USER_INFO_1, *PLSA_USER_INFO_1;

/**
 * @ingroup user
 * @brief User info structure -- level 2
 *
 * Describes everything about a user included in #__LSA_USER_INFO_1
 * in addition to attributes which describe the password expiry
 * and account status of the user.
 */
typedef struct __LSA_USER_INFO_2
{
#ifndef DOXYGEN
    union
    {
        struct
        {
#endif
            uid_t uid;
            gid_t gid;
            LW_PSTR pszName;
            LW_PSTR pszPasswd;
            LW_PSTR pszGecos;
            LW_PSTR pszShell;
            LW_PSTR pszHomedir;
            LW_PSTR pszSid;
            LW_PSTR pszDN;
            LW_PSTR pszUPN;
            LW_DWORD bIsGeneratedUPN;
            LW_DWORD bIsLocalUser;
            LW_PBYTE pLMHash;
            LW_DWORD dwLMHashLen;
            LW_PBYTE pNTHash;
            LW_DWORD dwNTHashLen;
#ifndef DOXYGEN
        };
        LSA_USER_INFO_1 info1;
    };
#endif
    /** @brief Number of days until the user's password will expire */
    LW_DWORD dwDaysToPasswordExpiry;
    /** @brief Whether the user's password has expired */
    LW_BOOLEAN bPasswordExpired;
    /** @brief Whether the user's password will never expire */
    LW_BOOLEAN bPasswordNeverExpires;
    /** @brief Whether the user should be prompted to change password */
    LW_BOOLEAN bPromptPasswordChange;
    /** @brief Whether the user can change password */
    LW_BOOLEAN bUserCanChangePassword;
    /** @brief Whether the account is disabled */
    LW_BOOLEAN bAccountDisabled;
    /** @brief Whether the account is expired */
    LW_BOOLEAN bAccountExpired;
    /** @brief Whether the account is locked */
    LW_BOOLEAN bAccountLocked;
} LSA_USER_INFO_2, *PLSA_USER_INFO_2;

typedef struct __LSA_USER_INFO_LIST
{
    LW_DWORD dwUserInfoLevel;
    LW_DWORD dwNumUsers;
    union _USER_INFO_LIST
    {
        PLSA_USER_INFO_0* ppInfoList0;
        PLSA_USER_INFO_1* ppInfoList1;
        PLSA_USER_INFO_2* ppInfoList2;
    } ppUserInfoList;
} LSA_USER_INFO_LIST, *PLSA_USER_INFO_LIST;

typedef struct __LSA_USER_MOD_INFO
{
    uid_t uid;

    struct _usermod_actions {
        LW_BOOLEAN bEnableUser;
        LW_BOOLEAN bDisableUser;
        LW_BOOLEAN bUnlockUser;
        LW_BOOLEAN bSetChangePasswordOnNextLogon;
        LW_BOOLEAN bSetPasswordNeverExpires;
        LW_BOOLEAN bSetPasswordMustExpire;
        LW_BOOLEAN bAddToGroups;
        LW_BOOLEAN bRemoveFromGroups;
        LW_BOOLEAN bSetAccountExpiryDate;
    } actions;

    LW_PSTR pszAddToGroups;
    LW_PSTR pszRemoveFromGroups;
    LW_PSTR pszExpiryDate;
} LSA_USER_MOD_INFO, *PLSA_USER_MOD_INFO;

typedef struct __LSA_GROUP_INFO_0
{
    gid_t gid;
    LW_PSTR pszName;
    LW_PSTR pszSid;
} LSA_GROUP_INFO_0, *PLSA_GROUP_INFO_0;

typedef struct __LSA_GROUP_INFO_1
{
    union
    {
        struct
        {
            gid_t gid;
            LW_PSTR pszName;
            LW_PSTR pszSid;
        };
        LSA_GROUP_INFO_0 info0;
    };
    LW_PSTR pszDN;
    LW_PSTR pszPasswd;
    LW_PSTR* ppszMembers;
} LSA_GROUP_INFO_1, *PLSA_GROUP_INFO_1;

typedef struct __LSA_GROUP_INFO_LIST
{
    LW_DWORD dwGroupInfoLevel;
    LW_DWORD dwNumGroups;
    union _GROUP_INFO_LIST
    {
        PLSA_GROUP_INFO_0* ppInfoList0;
        PLSA_GROUP_INFO_1* ppInfoList1;
    } ppGroupInfoList;
} LSA_GROUP_INFO_LIST, *PLSA_GROUP_INFO_LIST;

typedef struct __LSA_GROUP_MEMBER_INFO
{
    LW_PSTR pszDN;
    LW_PSTR pszSid;
} LSA_GROUP_MEMBER_INFO, *PLSA_GROUP_MEMBER_INFO;

typedef struct __LSA_GROUP_MOD_INFO
{
    gid_t gid;

    struct _groupmod_actions {
        LW_BOOLEAN bAddMembers;
        LW_BOOLEAN bRemoveMembers;
    } actions;

    LW_DWORD dwAddMembersNum;
    PLSA_GROUP_MEMBER_INFO pAddMembers;

    LW_DWORD dwRemoveMembersNum;
    PLSA_GROUP_MEMBER_INFO pRemoveMembers;
} LSA_GROUP_MOD_INFO, *PLSA_GROUP_MOD_INFO;

typedef struct __LSA_ENUM_OBJECTS_INFO
{
    LW_DWORD dwObjectInfoLevel;
    LW_DWORD dwNumMaxObjects;
    LW_PSTR pszGUID;
} LSA_ENUM_OBJECTS_INFO, *PLSA_ENUM_OBJECTS_INFO;

typedef struct __LSA_NSS_ARTEFACT_INFO_0
{
    LW_PSTR pszName;
    LW_PSTR pszValue;
} LSA_NSS_ARTEFACT_INFO_0, *PLSA_NSS_ARTEFACT_INFO_0;

typedef struct __LSA_NSS_ARTEFACT_INFO_LIST
{
    LW_DWORD dwNssArtefactInfoLevel;
    LW_DWORD dwNumNssArtefacts;
    union _NSS_ARTEFACT_INFO_LIST
    {
        PLSA_NSS_ARTEFACT_INFO_0* ppInfoList0;
    } ppNssArtefactInfoList;
} LSA_NSS_ARTEFACT_INFO_LIST, *PLSA_NSS_ARTEFACT_INFO_LIST;

typedef LW_UINT8 ADAccountType;

#define AccountType_NotFound 0
#define AccountType_Group 1
#define AccountType_User 2
#define AccountType_Domain 3

typedef struct __LSA_SID_INFO
{
    ADAccountType accountType;
    LW_PSTR pszSamAccountName;
    LW_PSTR pszDomainName;
} LSA_SID_INFO, *PLSA_SID_INFO;

typedef struct __LSA_FIND_NAMES_BY_SIDS
{
    size_t sCount;
    PLSA_SID_INFO pSIDInfoList;
    LW_CHAR chDomainSeparator;
} LSA_FIND_NAMES_BY_SIDS, *PLSA_FIND_NAMES_BY_SIDS;

typedef struct __LSA_METRIC_PACK_0
{
    LW_UINT64 failedAuthentications;
    LW_UINT64 failedUserLookupsByName;
    LW_UINT64 failedUserLookupsById;
    LW_UINT64 failedGroupLookupsByName;
    LW_UINT64 failedGroupLookupsById;
    LW_UINT64 failedOpenSession;
    LW_UINT64 failedCloseSession;
    LW_UINT64 failedChangePassword;
    LW_UINT64 unauthorizedAccesses;
} LSA_METRIC_PACK_0, *PLSA_METRIC_PACK_0;

typedef struct __LSA_METRIC_PACK_1
{
    LW_UINT64 successfulAuthentications;
    LW_UINT64 failedAuthentications;
    LW_UINT64 rootUserAuthentications;
    LW_UINT64 successfulUserLookupsByName;
    LW_UINT64 failedUserLookupsByName;
    LW_UINT64 successfulUserLookupsById;
    LW_UINT64 failedUserLookupsById;
    LW_UINT64 successfulGroupLookupsByName;
    LW_UINT64 failedGroupLookupsByName;
    LW_UINT64 successfulGroupLookupsById;
    LW_UINT64 failedGroupLookupsById;
    LW_UINT64 successfulOpenSession;
    LW_UINT64 failedOpenSession;
    LW_UINT64 successfulCloseSession;
    LW_UINT64 failedCloseSession;
    LW_UINT64 successfulChangePassword;
    LW_UINT64 failedChangePassword;
    LW_UINT64 unauthorizedAccesses;
} LSA_METRIC_PACK_1, *PLSA_METRIC_PACK_1;

typedef struct __LSA_METRIC_PACK
{
    LW_DWORD dwInfoLevel;
    union _METRIC_PACK
    {
        PLSA_METRIC_PACK_0 pMetricPack0;
        PLSA_METRIC_PACK_1 pMetricPack1;
    } pMetricPack;
} LSA_METRIC_PACK, *PLSA_METRIC_PACK;

typedef enum
{
    LSA_PROVIDER_MODE_UNKNOWN = 0,
    LSA_PROVIDER_MODE_UNPROVISIONED,
    LSA_PROVIDER_MODE_DEFAULT_CELL,
    LSA_PROVIDER_MODE_NON_DEFAULT_CELL,
    LSA_PROVIDER_MODE_LOCAL_SYSTEM
} LsaAuthProviderMode;

typedef enum
{
    LSA_AUTH_PROVIDER_SUBMODE_UNKNOWN = 0,
    LSA_AUTH_PROVIDER_SUBMODE_SCHEMA,
    LSA_AUTH_PROVIDER_SUBMODE_NONSCHEMA
} LsaAuthProviderSubMode;

typedef enum
{
    LSA_AUTH_PROVIDER_STATUS_UNKNOWN = 0,
    LSA_AUTH_PROVIDER_STATUS_ONLINE,
    LSA_AUTH_PROVIDER_STATUS_OFFLINE,
    LSA_AUTH_PROVIDER_STATUS_FORCED_OFFLINE
} LsaAuthProviderStatus;

typedef struct __LSA_DC_INFO
{
    LW_PSTR pszName;
    LW_PSTR pszAddress;
    LW_PSTR pszSiteName;
    LSA_DS_FLAGS dwFlags;
} LSA_DC_INFO, *PLSA_DC_INFO;

typedef struct __LW_LSA_TRUSTED_DOMAIN_INFO
{
    LW_PSTR pszDnsDomain;
    LW_PSTR pszNetbiosDomain;
    LW_PSTR pszTrusteeDnsDomain;
    LW_PSTR pszDomainSID;
    LW_PSTR pszDomainGUID;
    LW_PSTR pszForestName;
    LW_PSTR pszClientSiteName;
    LSA_TRUST_FLAG dwTrustFlags;
    LSA_TRUST_TYPE dwTrustType;
    LSA_TRUST_ATTRIBUTE dwTrustAttributes;
    LSA_TRUST_DIRECTION dwTrustDirection;
    LSA_TRUST_MODE dwTrustMode;
    LSA_DM_DOMAIN_FLAGS dwDomainFlags;
    PLSA_DC_INFO pDCInfo;
    PLSA_DC_INFO pGCInfo;
} LW_LSA_TRUSTED_DOMAIN_INFO, *PLW_LSA_TRUSTED_DOMAIN_INFO;

#ifndef LW_STRICT_NAMESPACE
typedef LW_LSA_TRUSTED_DOMAIN_INFO LSA_TRUSTED_DOMAIN_INFO;
typedef PLW_LSA_TRUSTED_DOMAIN_INFO PLSA_TRUSTED_DOMAIN_INFO;
#endif

typedef struct __LSA_AUTH_PROVIDER_STATUS
{
    LW_PSTR pszId;
    LsaAuthProviderMode mode;
    LsaAuthProviderSubMode subMode;
    LsaAuthProviderStatus status;
    LW_PSTR pszDomain;
    LW_PSTR pszForest;
    LW_PSTR pszSite;
    LW_PSTR pszCell;
    LW_DWORD dwNetworkCheckInterval;
    LW_DWORD dwNumTrustedDomains;
    PLW_LSA_TRUSTED_DOMAIN_INFO pTrustedDomainInfoArray;
} LSA_AUTH_PROVIDER_STATUS, *PLSA_AUTH_PROVIDER_STATUS;

typedef struct __LSA_VERSION
{
    LW_DWORD dwMajor;
    LW_DWORD dwMinor;
    LW_DWORD dwBuild;
} LSA_VERSION, *PLSA_VERSION;

typedef struct __LSASTATUS
{
    LW_DWORD dwUptime;

    LSA_VERSION version;

    LW_DWORD dwCount;
    PLSA_AUTH_PROVIDER_STATUS pAuthProviderStatusList;
} LSASTATUS, *PLSASTATUS;


/*
 * AuthenticateUserEx() parameters
 */

typedef enum
{
    LSA_AUTH_PLAINTEXT = 1,
    LSA_AUTH_CHAP
} LsaAuthType;

typedef struct __LSA_AUTH_CLEARTEXT_PARAM
{
    LW_PSTR pszPassword;
} LSA_AUTH_CLEARTEXT_PARAM, *PLSA_AUTH_CLEARTEXT_PARAM;

typedef struct __LW_LSA_DATA_BLOB
{
    LW_DWORD dwLen;
    LW_PBYTE pData;
} LW_LSA_DATA_BLOB, *PLW_LSA_DATA_BLOB;

#ifndef LW_STRICT_NAMESPACE
typedef LW_LSA_DATA_BLOB LSA_DATA_BLOB;
typedef PLW_LSA_DATA_BLOB PLSA_DATA_BLOB;
#endif

typedef struct __LSA_AUTH_CHAP_PARAM
{
    PLW_LSA_DATA_BLOB pChallenge;
    PLW_LSA_DATA_BLOB pLM_resp;
    PLW_LSA_DATA_BLOB pNT_resp;
} LSA_AUTH_CHAP_PARAM, *PLSA_AUTH_CHAP_PARAM;

typedef struct __LSA_AUTH_USER_PARAMS
{
    LsaAuthType AuthType;
    LW_PSTR pszAccountName;
    LW_PSTR pszDomain;
    LW_PSTR pszWorkstation;
    union _PASS {
        LSA_AUTH_CLEARTEXT_PARAM clear;
        LSA_AUTH_CHAP_PARAM chap;
    } pass;
} LSA_AUTH_USER_PARAMS, *PLSA_AUTH_USER_PARAMS;

#define LSA_MAX_SID_SUB_AUTHORITIES  15

typedef struct __LSA_SID_ATTRIB
{
    LW_PSTR pszSid;
    LW_DWORD dwAttrib;
} LSA_SID_ATTRIB, *PLSA_SID_ATTRIB;

typedef struct __LSA_RID_ATTRIB
{
    LW_UINT32 Rid;
    LW_DWORD dwAttrib;
} LSA_RID_ATTRIB, *PLSA_RID_ATTRIB;

#define LSA_SID_ATTR_GROUP_MANDATORY             0x00000001
#define LSA_SID_ATTR_GROUP_ENABLED_BY_DEFAULT    0x00000002
#define LSA_SID_ATTR_GROUP_ENABLED               0x00000004
#define LSA_SID_ATTR_GROUP_OWNER                 0x00000008
#define LSA_SID_ATTR_GROUP_USEFOR_DENY_ONLY      0x00000010
#define LSA_SID_ATTR_GROUP_RESOURCE              0x20000000
#define LSA_SID_ATTR_GROUP_LOGON_ID              0xC0000000

typedef struct __LSA_AUTH_USER_INFO
{
    LW_DWORD dwUserFlags;

    LW_PSTR pszAccount;
    LW_PSTR pszUserPrincipalName;
    LW_PSTR pszFullName;
    LW_PSTR pszDomain;
    LW_PSTR pszDnsDomain;

    LW_DWORD dwAcctFlags;
    PLW_LSA_DATA_BLOB pSessionKey;
    PLW_LSA_DATA_BLOB pLmSessionKey;

    LW_UINT16 LogonCount;
    LW_UINT16 BadPasswordCount;

    LW_INT64 LogonTime;
    LW_INT64 LogoffTime;
    LW_INT64 KickoffTime;
    LW_INT64 LastPasswordChange;
    LW_INT64 CanChangePassword;
    LW_INT64 MustChangePassword;

    LW_PSTR pszLogonServer;
    LW_PSTR pszLogonScript;
    LW_PSTR pszProfilePath;
    LW_PSTR pszHomeDirectory;
    LW_PSTR pszHomeDrive;

    LW_PSTR pszDomainSid;
    LW_DWORD dwUserRid;
    LW_DWORD dwPrimaryGroupRid;

    LW_DWORD dwNumRids;
    PLSA_RID_ATTRIB pRidAttribList;

    LW_DWORD dwNumSids;
    PLSA_SID_ATTRIB pSidAttribList;
} LSA_AUTH_USER_INFO, *PLSA_AUTH_USER_INFO;

/**
 * @ingroup connection
 * @brief Open connection to local LSASS server
 *
 * Creates a connection handle to the local LSASS server.
 *
 * @param[out] phConnection the created connection handle
 * @retval LSA_ERROR_SUCCESS success
 * @retval ECONNREFUSED the connection was refused
 * @retval ENOENT the local domain socket was not present
 */
LW_DWORD
LsaOpenServer(
    LW_PHANDLE phConnection
    );

LW_DWORD
LsaBuildLogInfo(
    LsaLogLevel maxAllowedLogLevel,
    LsaLogTarget logTarget,
    LW_PCSTR pszPath,
    PLSA_LOG_INFO* ppLogInfo
    );

LW_DWORD
LsaSetLogLevel(
    LW_HANDLE hLsaConnection,
    LsaLogLevel logLevel
    );

LW_DWORD
LsaGetLogInfo(
    LW_HANDLE hLsaConnection,
    PLSA_LOG_INFO* ppLogInfo
    );

LW_DWORD
LsaSetLogInfo(
    LW_HANDLE hLsaConnection,
    PLSA_LOG_INFO pLogInfo
    );

LW_VOID
LsaFreeLogInfo(
    PLSA_LOG_INFO pLogInfo
    );

LW_DWORD
LsaSetTraceFlags(
    LW_HANDLE hLsaConnection,
    PLSA_TRACE_INFO pTraceFlagArray,
    LW_DWORD dwNumFlags
    );

LW_DWORD
LsaEnumTraceFlags(
    LW_HANDLE hLsaConnection,
    PLSA_TRACE_INFO* ppTraceFlagArray,
    LW_PDWORD pdwNumFlags
    );

LW_DWORD
LsaGetTraceFlag(
    LW_HANDLE hLsaConnection,
    LW_DWORD dwTraceFlag,
    PLSA_TRACE_INFO* ppTraceFlag
    );

/**
 * @ingroup group
 * @brief Create new group
 *
 * Creates a new group in the local account database.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] pGroupInfo a group info structure
 * @param[in] dwGroupInfoLevel the info level of the provided group info structure
 * @retval LSA_ERROR_SUCCESS success
 * @retval EPERM the owner of the current process is not authorized to create groups
 */
LW_DWORD
LsaAddGroup(
    LW_HANDLE hLsaConnection,
    LW_PVOID pGroupInfo,
    LW_DWORD dwGroupInfoLevel
    );

LW_DWORD
LsaModifyGroup(
    LW_HANDLE hLsaConnection,
    PLSA_GROUP_MOD_INFO pGroupModInfo
    );

/**
 * @ingroup group
 * @brief Delete a group by ID
 *
 * Deletes a group from the local account database based on its UNIX group ID
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] gid the group ID of the group to delete
 * @retval LSA_ERROR_SUCCESS success
 * @retval EPERM the owner of the current process is not authorized to delete groups
 * @retval LSA_ERROR_NO_SUCH_GROUP the specified group ID did not match any local group
 */
LW_DWORD
LsaDeleteGroupById(
    LW_HANDLE hLsaConnection,
    gid_t gid
    );

/**
 * @ingroup group
 * @brief Delete a group by name
 *
 * Deletes a group from the local account database based on its name
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] pszName the name of the group to delete
 * @retval LSA_ERROR_SUCCESS success
 * @retval EPERM the owner of the current process is not authorized to delete groups
 * @retval LSA_ERROR_NO_SUCH_GROUP the specified group name did not match any local group
 */
LW_DWORD
LsaDeleteGroupByName(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszName
    );

/**
 * @ingroup user
 * @brief Look up group IDs by username
 *
 * Looks up the group IDs for the groups which a user is a member of based on the user's login name.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] pszUserName the login name of the user
 * @param[out] pdwGroupFound the number of groups find
 * @param[out] ppGidResults a heap-allocated list of group IDs
 * @retval LSA_ERROR_SUCCESS success
 * @retval LSA_ERROR_NO_SUCH_USER the specified user name did not match any known user
 */
LW_DWORD
LsaGetGidsForUserByName(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszUserName,
    LW_PDWORD pdwGroupFound,
    gid_t** ppGidResults
    );

/**
 * @ingroup user
 * @brief Look up groups by user ID
 *
 * Looks up information on groups which a user is a member of based on user's login name.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] pszUserName the login name of the user
 * @param[in] FindFlags options for the lookup operation
 * @param[in] dwGroupInfoLevel the desired info level for the returned group info structures
 * @param[out] pdwGroupsFound the number of groups find
 * @param[out] pppGroupInfoList a heap-allocated list of group info structures
 * @retval LSA_ERROR_SUCCESS success
 * @retval LSA_ERROR_NO_SUCH_USER the specified user ID did not match any known user
 */
LW_DWORD
LsaGetGroupsForUserByName(
    LW_IN LW_HANDLE hLsaConnection,
    LW_IN LW_PCSTR pszUserName,
    LW_IN LSA_FIND_FLAGS FindFlags,
    LW_IN LW_DWORD dwGroupInfoLevel,
    LW_OUT LW_PDWORD pdwGroupsFound,
    LW_OUT LW_PVOID** pppGroupInfoList
    );

/**
 * @ingroup user
 * @brief Look up groups by user ID
 *
 * Looks up information on groups which a user is a member of based on user ID.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] uid the user ID of the user
 * @param[in] FindFlags options for the lookup operation
 * @param[in] dwGroupInfoLevel the desired info level for the returned group info structures
 * @param[out] pdwGroupsFound the number of groups find
 * @param[out] pppGroupInfoList a heap-allocated list of group info structures
 * @retval LSA_ERROR_SUCCESS success
 * @retval LSA_ERROR_NO_SUCH_USER the specified user ID did not match any known user
 */
LW_DWORD
LsaGetGroupsForUserById(
    LW_HANDLE hLsaConnection,
    uid_t uid,
    LSA_FIND_FLAGS FindFlags,
    LW_DWORD dwGroupInfoLevel,
    LW_PDWORD pdwGroupsFound,
    LW_PVOID** pppGroupInfoList
    );

/**
 * @ingroup group
 * @brief Look up group by name
 *
 * Looks up information on a group by its name.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] pszGroupName the name of the group
 * @param[in] FindFlags options for the lookup operation
 * @param[in] dwGroupInfoLevel the desired info level for the returned group info structure
 * @param[out] ppGroupInfo a heap-allocated group info structure for the found group
 * @retval LSA_ERROR_SUCCESS success
 * @retval LSA_ERROR_NO_SUCH_GROUP the specified name did not match any known group
 */
LW_DWORD
LsaFindGroupByName(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszGroupName,
    LSA_FIND_FLAGS FindFlags,
    LW_DWORD dwGroupInfoLevel,
    LW_PVOID* ppGroupInfo
    );

/**
 * @ingroup group
 * @brief Look up group by ID
 *
 * Looks up information on a group by its group ID.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] gid the group ID of the group
 * @param[in] FindFlags options for the lookup operation
 * @param[in] dwGroupInfoLevel the desired info level for the returned group info structure
 * @param[out] ppGroupInfo a heap-allocated group info structure for the found group
 * @retval LSA_ERROR_SUCCESS success
 * @retval LSA_ERROR_NO_SUCH_GROUP the specified group ID did not match any known group
 */
LW_DWORD
LsaFindGroupById(
    LW_HANDLE hLsaConnection,
    gid_t gid,
    LSA_FIND_FLAGS FindFlags,
    LW_DWORD dwGroupInfoLevel,
    LW_PVOID* ppGroupInfo
    );

/**
 * @ingroup group
 * @brief Begin group enumeration
 *
 * Begins an enumeration of all known groups.  This function returns an
 * enumeration handle which can be used with #LsaEnumGroups() to fetch
 * lists of groups in increments of up to dwMaxNumGroups.
 *
 * You must call #LsaEndEnumGroups() on the enumeration handle when
 * finished with the enumeration.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] dwGroupInfoLevel the desired info level for the group info structures
 * returned during the enumeration
 * @param[in] dwMaxNumGroups the maximum number of group info structures to
 * return in each subsequent call to #LsaEnumGroups()
 * @param[in] FindFlags options for the lookup operation
 * @param[out] phResume the created enumeration handle
 * @retval #LSA_ERROR_SUCCESS success
 */
LW_DWORD
LsaBeginEnumGroups(
    LW_HANDLE hLsaConnection,
    LW_DWORD dwGroupInfoLevel,
    LW_DWORD dwMaxNumGroups,
    LSA_FIND_FLAGS FindFlags,
    LW_PHANDLE phResume
    );

/**
 * @ingroup group
 * @brief Begin group enumeration with online check option
 *
 * Begins an enumeration of all known groups.  This function returns an
 * enumeration handle which can be used with #LsaEnumGroups() to fetch
 * lists of groups in increments of up to dwMaxNumGroups.  Compared to
 * #LsaBeginEnumGroups(), it supports an extra option that allows the
 * query to be restricted to groups that are local or cached, avoiding
 * excess traffic when large numbers of groups are present in a networked
 * identity database (e.g. AD).
 *
 * You must call #LsaEndEnumGroups() on the enumeration handle when
 * finished with the enumeration.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] dwGroupInfoLevel the desired info level for the group info structures
 * returned during the enumeration
 * @param[in] dwMaxNumGroups the maximum number of group info structures to
 * return in each subsequent call to #LsaEnumGroups()
 * @param[in] bCheckGroupMembersOnline TRUE if networked databases should be
 * consulted, FALSE if only local databases or caches should be searched
 * @param[in] FindFlags options for the lookup operation
 * @param[out] phResume the created enumeration handle
 * @retval #LSA_ERROR_SUCCESS success
 */
LW_DWORD
LsaBeginEnumGroupsWithCheckOnlineOption(
    LW_HANDLE hLsaConnection,
    LW_DWORD dwGroupInfoLevel,
    LW_DWORD dwMaxNumGroups,
    LW_BOOLEAN bCheckGroupMembersOnline,
    LSA_FIND_FLAGS FindFlags,
    LW_PHANDLE phResume
    );

/**
 * @ingroup group
 * @brief Retrieve next list of groups during enumeration
 *
 * Retrieves the next list of groups for an in-progress enumeration.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] hResume the enumeration handle
 * @param[out] pdsNumGroupsFound the number of groups returned
 * @param[out] pppGroupInfoList a heap-allocated list of group info structures
 * of the level specified in the call to #LsaBeginEnumGroups().  It should be
 * freed with #LsaFreeGroupInfoList().
 * @retval #LSA_ERROR_SUCCESS success
 */
LW_DWORD
LsaEnumGroups(
    LW_HANDLE hLsaConnection,
    LW_HANDLE hResume,
    LW_PDWORD pdwNumGroupsFound,
    LW_PVOID** pppGroupInfoList
    );

/**
 * @ingroup group
 * @brief End group enumeration
 *
 * Ends a group enumeration, releasing any associated resources.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in,out] hResume the enumeration handle
 * @retval #LSA_ERROR_SUCCESS success
 */
LW_DWORD
LsaEndEnumGroups(
    LW_HANDLE hLsaConnection,
    LW_HANDLE hResume
    );

/**
 * @ingroup group
 * @brief Free a list of group info structures
 *
 * Frees a list of group info structures of the specified level.
 *
 * @param[in] dwLevel the info level of the structures
 * @param[in,out] pGroupInfoList the info list
 * @param[dwNumGroups] dwNumGroups the number of elements in the list
 */
LW_VOID
LsaFreeGroupInfoList(
    LW_DWORD dwLevel,
    LW_PVOID* pGroupInfoList,
    LW_DWORD dwNumGroups
    );

/**
 * @ingroup group
 * @brief Free a group info structure
 *
 * Frees a single group info structure of the specified level.
 *
 * @param[in] dwLevel the info level of the structures
 * @param[in,out] pGroupInfo the info structure
 */
LW_VOID
LsaFreeGroupInfo(
    LW_DWORD dwLevel,
    LW_PVOID pGroupInfo
    );

/* FIXME: should these be public? */
#ifndef DOXYGEN

LW_VOID
LsaFreeEnumObjectsInfo(
    PLSA_ENUM_OBJECTS_INFO pInfo
    );

LW_VOID
LsaFreeNSSArtefactInfoList(
    LW_DWORD dwLevel,
    LW_PVOID* pNSSArtefactInfoList,
    LW_DWORD dwNumNSSArtefacts
    );
#endif

LW_VOID
LsaFreeNSSArtefactInfo(
    LW_DWORD dwLevel,
    LW_PVOID pNSSArtefactInfo
    );

LW_DWORD
LsaAddUser(
    LW_HANDLE hLsaConnection,
    LW_PVOID pUserInfo,
    LW_DWORD dwUserInfoLevel
    );

LW_DWORD
LsaModifyUser(
    LW_HANDLE hLsaConnection,
    PLSA_USER_MOD_INFO pUserModInfo
    );

LW_DWORD
LsaDeleteUserById(
    LW_HANDLE hLsaConnection,
    uid_t uid
    );

LW_DWORD
LsaDeleteUserByName(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszName
    );

/**
 * @ingroup user
 * @brief Look up user by name
 *
 * Looks up information on a user by its name.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] pszGroupName the name of the user
 * @param[in] FindFlags options for the lookup operation
 * @param[in] dwUserInfoLevel the desired info level for the returned user info structure
 * @param[out] ppGroupInfo a heap-allocated group info structure for the found group
 * @retval LSA_ERROR_SUCCESS success
 * @retval LSA_ERROR_NO_SUCH_GROUP the specified name did not match any known group
 */
LW_DWORD
LsaFindUserByName(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszName,
    LW_DWORD dwUserInfoLevel,
    LW_PVOID* ppUserInfo
    );

/**
 * @ingroup user
 * @brief Look up user by ID
 *
 * Looks up information on a user by its user ID.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] uid the user ID of the user
 * @param[in] FindFlags options for the lookup operation
 * @param[in] dwUserInfoLevel the desired info level for the returned user info structure
 * @param[out] ppUserInfo a heap-allocated user info structure for the found user
 * @retval LSA_ERROR_SUCCESS success
 * @retval LSA_ERROR_NO_SUCH_USER the specified user ID did not match any known user
 */
LW_DWORD
LsaFindUserById(
    LW_HANDLE hLsaConnection,
    uid_t uid,
    LW_DWORD dwUserInfoLevel,
    LW_PVOID* ppUserInfo
    );

LW_DWORD
LsaGetNamesBySidList(
    LW_IN LW_HANDLE hLsaConnection,
    LW_IN size_t sCount,
    LW_IN LW_PSTR* ppszSidList,
    LW_OUT PLSA_SID_INFO* ppSIDInfoList,
    LW_OUT LW_OPTIONAL LW_CHAR *pchDomainSeparator
    );

LW_VOID
LsaFreeSIDInfoList(
    PLSA_SID_INFO ppSIDInfoList,
    size_t stNumSID
    );

LW_VOID
LsaFreeSIDInfo(
    PLSA_SID_INFO pSIDInfo
    );

/**
 * @ingroup user
 * @brief Begin user enumeration
 *
 * Begins an enumeration of all known users.  This function returns an
 * enumeration handle which can be used with #LsaEnumUsers() to fetch
 * lists of users in increments of up to dwMaxNumUsers.
 *
 * You must call #LsaEndEnumUsers() on the enumeration handle when
 * finished with the enumeration.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in] dwUserInfoLevel the desired info level for the user info structures
 * returned during the enumeration
 * @param[in] dwMaxNumUsers the maximum number of user info structures to
 * return in each subsequent call to #LsaEnumUsers()
 * @param[in] FindFlags options for the lookup operation
 * @param[out] phResume the created enumeration handle
 * @retval #LSA_ERROR_SUCCESS success
 */
LW_DWORD
LsaBeginEnumUsers(
    LW_HANDLE hLsaConnection,
    LW_DWORD dwUserInfoLevel,
    LW_DWORD dwMaxNumUsers,
    LSA_FIND_FLAGS FindFlags,
    LW_PHANDLE phResume
    );

/**
 * @ingroup user
 * @brief End user enumeration
 *
 * Ends a user enumeration, releasing any associated resources.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in,out] hResume the enumeration handle
 * @retval #LSA_ERROR_SUCCESS success
 */
LW_DWORD
LsaEnumUsers(
    LW_HANDLE hLsaConnection,
    LW_HANDLE hResume,
    LW_PDWORD pdwNumUsersFound,
    LW_PVOID** pppUserInfoList
    );

/**
 * @ingroup user
 * @brief End user enumeration
 *
 * Ends a user enumeration, releasing any associated resources.
 *
 * @param[in] hLsaConnection the connection handle
 * @param[in,out] hResume the enumeration handle
 * @retval #LSA_ERROR_SUCCESS success
 */
LW_DWORD
LsaEndEnumUsers(
    LW_HANDLE hLsaConnection,
    LW_HANDLE hResume
    );

/**
 * @ingroup user
 * @brief Free a list of user info structures
 *
 * Frees a list of user info structures of the specified level.
 *
 * @param[in] dwLevel the info level of the structures
 * @param[in,out] pUserInfoList the info list
 * @param[dwNumUsers] dwNumUsers the number of elements in the list
 */
LW_VOID
LsaFreeUserInfoList(
    LW_DWORD dwLevel,
    LW_PVOID* pUserInfoList,
    LW_DWORD dwNumUsers
    );


/**
 * @ingroup user
 * @brief Free a user info structure
 *
 * Frees a single user info structure of the specified level.
 *
 * @param[in] dwLevel the info level of the structures
 * @param[in,out] pUserInfo the info structure
 */
LW_VOID
LsaFreeUserInfo(
    LW_DWORD dwLevel,
    LW_PVOID pUserInfo
    );

LW_DWORD
LsaAuthenticateUser(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszLoginName,
    LW_PCSTR pszPassword
    );

LW_DWORD
LsaFreeAuthUserInfo(
    PLSA_AUTH_USER_INFO* ppAuthUserInfo
    );

LW_DWORD
LsaFreeAuthUserParams(
    PLSA_AUTH_USER_PARAMS* ppAuthUserParams
    );

LW_DWORD
LsaAuthenticateUserEx(
    LW_IN LW_HANDLE hLsaConnection,
    LW_IN LSA_AUTH_USER_PARAMS* pParams,
    LW_OUT PLSA_AUTH_USER_INFO* ppUserInfo
    );

LW_DWORD
LsaValidateUser(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszLoginName,
    LW_PCSTR pszPassword
    );

LW_DWORD
LsaCheckUserInList(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszLoginName,
    LW_PCSTR pszListName
    );

LW_DWORD
LsaChangePassword(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszLoginName,
    LW_PCSTR pszNewPassword,
    LW_PCSTR pszOldPassword
    );

LW_DWORD
LsaSetPassword(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszLoginName,
    LW_PCSTR pszNewPassword
    );

LW_DWORD
LsaOpenSession(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszLoginId
    );

LW_DWORD
LsaCloseSession(
    LW_HANDLE hLsaConnection,
    LW_PCSTR pszLoginId
    );

LW_DWORD
LsaGetMetrics(
    LW_HANDLE hLsaConnection,
    LW_DWORD dwInfoLevel,
    LW_PVOID* ppMetricPack
    );

LW_DWORD
LsaGetStatus(
   LW_HANDLE hLsaConnection,
   PLSASTATUS* ppLsaStatus
   );

LW_DWORD
LsaRefreshConfiguration(
   LW_HANDLE hLsaConnection
   );

LW_VOID
LsaFreeStatus(
   PLSASTATUS pLsaStatus
   );

/**
 * @ingroup connection
 * @brief Closes connection to LSASS server
 *
 * Closes a connection handle opened with #LsaOpenServer().
 *
 * @param[in,out] hConnection the connection handle to close
 * @retval LSA_ERROR_SUCCESS success
 * @retval EINVAL the handle was invalid
 */
LW_DWORD
LsaCloseServer(
    LW_HANDLE hConnection
    );

/**
 * @ingroup utility
 * @brief Get error code description
 *
 * Retrives a human-readable description of an LSASS
 * error code.  The description will be copied into
 * the specified buffer, but will be truncated if it
 * does not fit.
 *
 * @param[in] dwError the error code
 * @param[out] pszBuffer the buffer into which the description will be copied
 * @param[in] stBufSize the size of the provided buffer
 * @return the full size of the error string, which may be larger than the
 * provided buffer
 */
size_t
LsaGetErrorString(
    LW_DWORD dwError,
    LW_PSTR pszBuffer,
    size_t stBufSize
    );

/**
 * @ingroup utility
 * @brief Free a memory block
 *
 * Frees a single memory block allocated by an LSASS client API
 * function.  This may be used to peform fine-grained management
 * of heap-allocated structures in lieu of functions such as
 * #LsaFreeGroupInfo().
 *
 * @param[in,out] pMemory a pointer to the memory block
 */
LW_VOID
LsaFreeMemory(
    LW_PVOID pMemory
    );

LW_DWORD
LsaGetErrorMessageForLoggingEvent(
    LW_DWORD dwError,
    LW_PSTR* ppszErrorMsg
    );


/*
 * LW_LSA_DATA_BLOB access functions and methods
 */

LW_DWORD
LsaDataBlobAllocate(
    PLW_LSA_DATA_BLOB* ppBlob,
    LW_DWORD dwSize
    );

LW_DWORD
LsaDataBlobFree(
    PLW_LSA_DATA_BLOB* ppBlob
    );

LW_DWORD
LsaDataBlobStore(
    PLW_LSA_DATA_BLOB* ppBlob,
    LW_DWORD dwSize,
    const LW_PBYTE pBuffer
    );

LW_DWORD
LsaDataBlobCopy(
    PLW_LSA_DATA_BLOB* ppDst,
    PLW_LSA_DATA_BLOB pSrc
    );

LW_DWORD
LsaDataBlobLength(
    PLW_LSA_DATA_BLOB pBlob
    );

LW_PBYTE
LsaDataBlobBuffer(
    PLW_LSA_DATA_BLOB pBlob
    );


//
// NIS Map Routines
//

LW_DWORD
LsaFindNSSArtefactByKey(
    LW_HANDLE hLsaConnection,
    LW_DWORD dwMapInfoLevel,
    LW_PCSTR pszKeyName,
    LW_PCSTR pszMapName,
    LSA_NIS_MAP_QUERY_FLAGS dwFlags,
    LW_PVOID* ppNSSArtefactInfo
    );

LW_DWORD
LsaBeginEnumNSSArtefacts(
    LW_HANDLE hLsaConnection,
    LW_DWORD dwInfoLevel,
    LW_PCSTR pszMapName,
    LSA_NIS_MAP_QUERY_FLAGS dwFlags,
    LW_DWORD dwMaxNumNSSArtefacts,
    LW_PHANDLE phResume
    );

LW_DWORD
LsaEnumNSSArtefacts(
    LW_HANDLE hLsaConnection,
    LW_HANDLE hResume,
    LW_PDWORD pdwNumNSSArtefactsFound,
    LW_PVOID** pppNSSArtefactInfoList
    );

LW_DWORD
LsaEndEnumNSSArtefacts(
    LW_HANDLE hLsaConnection,
    LW_HANDLE hResume
    );

//
// Provider-Specific IOCTL Support
//

LW_DWORD
LsaProviderIoControl(
    LW_IN LW_HANDLE hLsaConnection,
    LW_IN LW_PCSTR pszProviderId,
    LW_IN LW_DWORD dwIoControlCode,
    LW_IN LW_DWORD dwInputBufferSize,
    LW_IN LW_PVOID pInputBuffer,
    LW_OUT LW_OPTIONAL LW_DWORD* pdwOutputBufferSize,
    LW_OUT LW_OPTIONAL LW_PVOID* ppOutputBuffer
    );

//
// GSS Support
//

typedef struct _LSA_SEC_BUFFER {
    LW_USHORT length;
    LW_USHORT maxLength;
    LW_PBYTE buffer;
} LSA_SEC_BUFFER, *PLSA_SEC_BUFFER;

/* static buffer secbufer */
#define LSA_SEC_BUFFER_S_BUFFER_SIZE 24

typedef struct _LSA_SEC_BUFFER_S {
    LW_USHORT length;
    LW_USHORT maxLength;
    LW_BYTE buffer[LSA_SEC_BUFFER_S_BUFFER_SIZE];
} LSA_SEC_BUFFER_S, *PLSA_SEC_BUFFER_S;

#ifndef LW_STRICT_NAMESPACE
typedef LSA_SEC_BUFFER SEC_BUFFER;
typedef PLSA_SEC_BUFFER PSEC_BUFFER;

#define S_BUFLEN LSA_SEC_BUFFER_S_BUFFER_SIZE

typedef LSA_SEC_BUFFER_S SEC_BUFFER_S;
typedef PLSA_SEC_BUFFER_S PSEC_BUFFER_S;
#endif

LW_DWORD
LsaGSSBuildAuthMessage(
    LW_HANDLE hLsaConnection,
    PLSA_SEC_BUFFER credentials,
    PLSA_SEC_BUFFER_S serverChallenge,
    PLSA_SEC_BUFFER targetInfo,
    LW_ULONG negotiateFlags,
    PLSA_SEC_BUFFER authenticateMessage,
    PLSA_SEC_BUFFER_S baseSessionKey
    );

LW_DWORD
LsaGSSValidateAuthMessage(
    LW_HANDLE hLsaConnection,
    LW_ULONG negFlags,
    PLSA_SEC_BUFFER_S serverChallenge,
    PLSA_SEC_BUFFER targetInfo,
    PLSA_SEC_BUFFER authenticateMessage,
    PLSA_SEC_BUFFER_S baseSessionKey
    );

#endif /* __LSA_H__ */


/*
local variables:
mode: c
c-basic-offset: 4
indent-tabs-mode: nil
tab-width: 4
end:
*/
