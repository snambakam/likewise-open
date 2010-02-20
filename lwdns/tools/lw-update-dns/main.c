/* Editor Settings: expandtabs and use 4 spaces for indentation
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 * -*- mode: c, c-basic-offset: 4 -*- */

#include "includes.h"

typedef struct _ARGS {
    BOOLEAN bShowArguments;
    BOOLEAN bUseMachineCredentials;
    PSTR pszHostname;
    PSTR pszHostDnsSuffix;
    PSOCKADDR_IN pAddressArray;
    DWORD dwAddressCount;
    LWDNSLogLevel LogLevel;
    PFN_LWDNS_LOG_MESSAGE pfnLogger;
} ARGS, *PARGS;

#define HAVE_MORE_ARGS(Argc, LastArgIndex, ArgsNeeded) \
    (((Argc) - (LastArgIndex)) > (ArgsNeeded))

static
DWORD
ParseArgs(
    IN int argc,
    IN const char* argv[],
    OUT PARGS pArgs
    );

static
DWORD
GetHostname(
    PSTR* ppszHostname
    );

static
DWORD
GetDnsSuffixByHostname(
    IN PCSTR pszHostname,
    OUT PSTR *ppszHostDnsSuffix
    );

static
DWORD
GetAllInterfaceAddresses(
    OUT PSOCKADDR_IN* ppAddressArray,
    OUT PDWORD pdwAddressCount
    );

static
DWORD
SetupCredentials(
    IN PCSTR pszHostname,
    OUT PSTR* ppszHostDnsSuffix
    );

static
VOID
CleanupCredentials(
    VOID
    );

static
VOID
LogMessage(
    LWDNSLogLevel logLevel,
    PCSTR         pszFormat,
    va_list       msgList
    );

static
VOID
ShowUsage(
    PCSTR pszProgramName
    );

static
VOID
PrintError(
    IN DWORD dwError
    );

int
main(
    int argc,
    const char* argv[]
    )
{
    DWORD dwError = 0;
    ARGS args = { 0 };
    PSTR pszHostname = NULL;
    PSTR pszHostDnsSuffixFromCreds = NULL;
    PSTR pszHostDnsSuffixFromHostname = NULL;
    PCSTR pszUseDnsSuffix = NULL;
    PCSTR pszUseHostname = NULL;
    PSTR pszHostFQDN = NULL;
    PSTR pszZone = NULL;
    PLW_NS_INFO pNameServerInfos = NULL;
    DWORD dwNameServerInfoCount = 0;
    HANDLE hDNSServer = NULL;
    DWORD iNS = 0;
    BOOLEAN bDNSUpdated = FALSE;
    DWORD iAddr = 0;

    dwError = ParseArgs(
                    argc,
                    argv,
                    &args);
    BAIL_ON_LWDNS_ERROR(dwError);

    dwError = LWNetExtendEnvironmentForKrb5Affinity(TRUE);
    BAIL_ON_LWDNS_ERROR(dwError);

    dwError = DNSInitialize();
    BAIL_ON_LWDNS_ERROR(dwError);

    if (args.pfnLogger)
    {
        DNSSetLogParameters(args.LogLevel, args.pfnLogger);
    }

    dwError = GetHostname(&pszHostname);
    BAIL_ON_LWDNS_ERROR(dwError);

    if (args.bUseMachineCredentials)
    {
        if (geteuid() != 0)
        {
            fprintf(stderr, "Please retry with super-user privileges\n");
            exit(EACCES);
        }

        dwError = SetupCredentials(pszHostname, &pszHostDnsSuffixFromCreds);
        BAIL_ON_LWDNS_ERROR(dwError);
    }

    DNSStrToUpper(args.pszHostDnsSuffix);
    pszUseDnsSuffix = args.pszHostDnsSuffix;
    if (!pszUseDnsSuffix)
    {
        DNSStrToUpper(pszHostDnsSuffixFromCreds);
        pszUseDnsSuffix = pszHostDnsSuffixFromCreds;
    }
    if (!pszUseDnsSuffix)
    {
        dwError = GetDnsSuffixByHostname(
                        pszHostname,
                        &pszHostDnsSuffixFromHostname);
        if (dwError)
        {
            fprintf(stderr, "Failed to get DNS suffix for host '%s'\n", pszHostname);
        }
        BAIL_ON_LWDNS_ERROR(dwError);

        DNSStrToUpper(pszHostDnsSuffixFromHostname);
        pszUseDnsSuffix = pszHostDnsSuffixFromHostname;
    }

    pszUseHostname = args.pszHostname;
    if (!pszUseHostname)
    {
        pszUseHostname = pszHostname;
    }

    dwError = DNSAllocateMemory(
                    strlen(pszUseHostname) + strlen(pszUseDnsSuffix) + 2,
                    (PVOID*)&pszHostFQDN);
    BAIL_ON_LWDNS_ERROR(dwError);

    sprintf(pszHostFQDN, "%s.%s", pszUseHostname, pszUseDnsSuffix);
    DNSStrToLower(pszHostFQDN);

    if (args.bShowArguments)
    {
        printf("Using FQDN %s with the following addresses:\n", pszHostFQDN);
        for (iAddr = 0; iAddr < args.dwAddressCount; iAddr++)
        {
            PSOCKADDR_IN pSockAddr = &args.pAddressArray[iAddr];
            printf("  %s\n", inet_ntoa(pSockAddr->sin_addr));
        }
    }

    dwError = DNSGetNameServers(
                    pszUseDnsSuffix,
                    &pszZone,
                    &pNameServerInfos,
                    &dwNameServerInfoCount);
    BAIL_ON_LWDNS_ERROR(dwError);

    if (!dwNameServerInfoCount)
    {
        dwError = LWDNS_ERROR_NO_NAMESERVER;
        BAIL_ON_LWDNS_ERROR(dwError);
    }

    for (iNS = 0; !bDNSUpdated && (iNS < dwNameServerInfoCount); iNS++)
    {
        PSTR   pszNameServer = NULL;
        PLW_NS_INFO pNSInfo = NULL;

        pNSInfo = &pNameServerInfos[iNS];
        pszNameServer = pNSInfo->pszNSHostName;

        if (hDNSServer)
        {
            DNSClose(hDNSServer);
        }

        LWDNS_LOG_INFO("Attempting to update name server [%s]", pszNameServer);

        dwError = DNSOpen(
                        pszNameServer,
                        DNS_TCP,
                        &hDNSServer);
        if (dwError)
        {
            LWDNS_LOG_ERROR(
                    "Failed to open connection to Name Server [%s]. [Error code:%d]",
                    pszNameServer,
                    dwError);
            dwError = 0;

            continue;
        }

        dwError = DNSUpdateSecure(
                        hDNSServer,
                        pszNameServer,
                        pszZone,
                        pszHostFQDN,
                        args.dwAddressCount,
                        args.pAddressArray);
        if (dwError)
        {
            LWDNS_LOG_ERROR(
                    "Failed to update Name Server [%s]. [Error code:%d]",
                    pszNameServer,
                    dwError);
            dwError = 0;
            
            continue;
        }

        bDNSUpdated = TRUE;
    }

    if (!bDNSUpdated)
    {
        dwError = LWDNS_ERROR_UPDATE_FAILED;
        BAIL_ON_LWDNS_ERROR(dwError);
    }

    printf("A record successfully updated in DNS\n");

    bDNSUpdated = FALSE;

    for (iAddr = 0; iAddr < args.dwAddressCount; iAddr++)
    {
        PSOCKADDR_IN pSockAddr = &args.pAddressArray[iAddr];

        dwError = DNSUpdatePtrSecure(
                        pSockAddr,
                        pszHostFQDN);
        if (dwError)
        {
            printf("Unable to register reverse PTR record address %s with hostname %s\n",
                    inet_ntoa(pSockAddr->sin_addr), pszHostFQDN);
            dwError = 0;
        }
        else
        {
            bDNSUpdated = TRUE;
        }
    }
    
    if (bDNSUpdated)
    {
        printf("PTR records successfully updated in DNS\n");
    }

cleanup:
    if (hDNSServer)
    {
        DNSClose(hDNSServer);
    }

    if (pNameServerInfos)
    {
        DNSFreeNameServerInfoArray(
                pNameServerInfos,
                dwNameServerInfoCount);
    }

    LWDNS_SAFE_FREE_STRING(pszZone);
    LWDNS_SAFE_FREE_STRING(pszHostFQDN);
    LWDNS_SAFE_FREE_STRING(pszHostDnsSuffixFromCreds);
    LWDNS_SAFE_FREE_STRING(pszHostDnsSuffixFromHostname);
    LWDNS_SAFE_FREE_STRING(pszHostname);

    LWDNS_SAFE_FREE_STRING(args.pszHostname);
    LWDNS_SAFE_FREE_STRING(args.pszHostDnsSuffix);
    LWDNS_SAFE_FREE_MEMORY(args.pAddressArray);

    DNSShutdown();

    CleanupCredentials();

    return dwError;

error:
    PrintError(dwError);

    goto cleanup;
}

static
DWORD
ParseArgs(
    IN int argc,
    IN const char* argv[],
    OUT PARGS pArgs
    )
{
    DWORD dwError = 0;
    DWORD iArg = 0;
    PCSTR pszHostFQDN = NULL;
    PSTR pszHostname = NULL;
    PSTR pszHostDnsSuffix = NULL;
    BOOLEAN bShowArguments = FALSE;
    BOOLEAN bUseMachineCredentials = TRUE;
    PSOCKADDR_IN pAddressArray = NULL;
    DWORD dwAddressCount = 0;
    LWDNSLogLevel LogLevel = LWDNS_LOG_LEVEL_ERROR;
    PFN_LWDNS_LOG_MESSAGE pfnLogger = NULL;
    PCSTR pszProgramName = "lw-update-dns";

    for (iArg = 1; iArg < argc; iArg++)
    {
        PCSTR pszArg = argv[iArg];

        if (!strcasecmp(pszArg, "-h") ||
            !strcasecmp(pszArg, "--help"))
        {
            ShowUsage(pszProgramName);
            exit(0);
        }
        else if (!strcasecmp(pszArg, "--loglevel"))
        {
            if (!HAVE_MORE_ARGS(argc, iArg, 1))
            {
                fprintf(stderr, "Missing argument for %s option.\n", pszArg);
                ShowUsage(pszProgramName);
                exit(1);
            }

            pszArg = argv[iArg + 1];
            iArg++;

            if (!strcasecmp(pszArg, "error"))
            {
                LogLevel = LWDNS_LOG_LEVEL_ERROR;
                pfnLogger = LogMessage;
            }
            else if (!strcasecmp(pszArg, "warning"))
            {
                LogLevel = LWDNS_LOG_LEVEL_WARNING;
                pfnLogger = LogMessage;
            }
            else if (!strcasecmp(pszArg, "info"))
            {
                LogLevel = LWDNS_LOG_LEVEL_INFO;
                pfnLogger = LogMessage;
            }
            else if (!strcasecmp(pszArg, "verbose"))
            {
                LogLevel = LWDNS_LOG_LEVEL_VERBOSE;
                pfnLogger = LogMessage;
            }
            else if (!strcasecmp(pszArg, "debug"))
            {
                LogLevel = LWDNS_LOG_LEVEL_DEBUG;
                pfnLogger = LogMessage;
            }
            else
            {
                fprintf(stderr, "Invalid log level: %s\n", pszArg);
                ShowUsage(pszProgramName);
                exit(1);
            }
        }
        else if (!strcasecmp(pszArg, "--ipaddress"))
        {
            PSOCKADDR_IN pSockAddr = NULL;

            if (!HAVE_MORE_ARGS(argc, iArg, 1))
            {
                fprintf(stderr, "Missing argument for %s option.\n", pszArg);
                ShowUsage(pszProgramName);
                exit(1);
            }

            pszArg = argv[iArg + 1];
            iArg++;

            dwError = DNSReallocMemory(
                            pAddressArray,
                            OUT_PPVOID(&pAddressArray),
                            sizeof(pAddressArray[0]) * (dwAddressCount + 1));
            BAIL_ON_LWDNS_ERROR(dwError);

            pSockAddr = &pAddressArray[dwAddressCount];
            pSockAddr->sin_family = AF_INET;
            if (!inet_aton(pszArg, &pSockAddr->sin_addr))
            {
                fprintf(stderr, "Invalid IP address: %s\n", pszArg);
                exit(1);
            }

            dwAddressCount++;
        }
        else if (!strcasecmp(pszArg, "--fqdn"))
        {
            PCSTR pszDot = NULL;

            if (!HAVE_MORE_ARGS(argc, iArg, 1))
            {
                fprintf(stderr, "Missing argument for %s option.\n", pszArg);
                ShowUsage(pszProgramName);
                exit(1);
            }

            if (pszHostFQDN)
            {
                fprintf(stderr, "Can only specify one %s option.\n", pszArg);
                ShowUsage(pszProgramName);
                exit(1);
            }

            pszArg = argv[iArg + 1];
            iArg++;

            pszHostFQDN = pszArg;

            pszDot = strchr(pszHostFQDN, '.');
            if (!pszDot || !pszDot[1])
            {
                fprintf(stderr, "Invalid FQDN: %s\n", pszArg);
                exit(1);
            }

            dwError = DNSAllocateString(
                            &pszDot[1],
                            &pszHostDnsSuffix);
            BAIL_ON_LWDNS_ERROR(dwError);

            dwError = DNSAllocateString(
                            pszHostFQDN,
                            &pszHostname);
            BAIL_ON_LWDNS_ERROR(dwError);

            pszHostname[pszDot - pszHostFQDN] = 0;
        }
        else if (!strcasecmp(pszArg, "--nocreds"))
        {
            bUseMachineCredentials = FALSE;
        }
        else if (!strcasecmp(pszArg, "--show"))
        {
            bShowArguments = TRUE;
        }
        else
        {
            fprintf(stderr, "Unexpected argument: %s\n", pszArg);
            ShowUsage(pszProgramName);
            exit(1);
        }
    }

    if (!pAddressArray)
    {
        dwError = GetAllInterfaceAddresses(&pAddressArray, &dwAddressCount);
        if (dwError)
        {
            fprintf(stderr, "Failed to get interface addresses.\n");
            BAIL_ON_LWDNS_ERROR(dwError);
        }
    }
    
cleanup:

    memset(pArgs, 0, sizeof(*pArgs));

    pArgs->bShowArguments = bShowArguments;
    pArgs->bUseMachineCredentials = bUseMachineCredentials;
    pArgs->pszHostname = pszHostname;
    pArgs->pszHostDnsSuffix = pszHostDnsSuffix;
    pArgs->pAddressArray = pAddressArray;
    pArgs->dwAddressCount = dwAddressCount;
    pArgs->LogLevel = LogLevel;
    pArgs->pfnLogger = pfnLogger;

    return dwError;

error:

    bShowArguments = FALSE;
    bUseMachineCredentials = TRUE;
    LWDNS_SAFE_FREE_STRING(pszHostname);
    LWDNS_SAFE_FREE_STRING(pszHostDnsSuffix);
    LWDNS_SAFE_FREE_MEMORY(pAddressArray);
    dwAddressCount = 0;
    LogLevel = LWDNS_LOG_LEVEL_ERROR;
    pfnLogger = NULL;

    goto cleanup;
}

static
DWORD
GetHostname(
    PSTR* ppszHostname
    )
{
    DWORD dwError = 0;
    CHAR szBuffer[256];
    PSTR pszLocal = NULL;
    PSTR pszDot = NULL;
    int len = 0;
    PSTR pszHostname = NULL;

    if ( gethostname(szBuffer, sizeof(szBuffer)) != 0 )
    {
        dwError = errno;
        BAIL_ON_LWDNS_ERROR(dwError);
    }

    len = strlen(szBuffer);
    if ( len > strlen(".local") )
    {
        pszLocal = &szBuffer[len - strlen(".local")];
        if ( !strcasecmp( pszLocal, ".local" ) )
        {
            pszLocal[0] = '\0';
        }
    }

    /* Test to see if the name is still dotted.
     * If so we will chop it down to just the
     * hostname field.
     */
    pszDot = strchr(szBuffer, '.');
    if ( pszDot )
    {
        pszDot[0] = '\0';
    }

    dwError = DNSAllocateString(
                    szBuffer,
                    &pszHostname);
    BAIL_ON_LWDNS_ERROR(dwError);

    *ppszHostname = pszHostname;

cleanup:

    return dwError;

error:

    LWDNS_SAFE_FREE_STRING(pszHostname);

    *ppszHostname = NULL;

    goto cleanup;
}

static
DWORD
GetDnsSuffixByHostname(
    IN PCSTR pszHostname,
    OUT PSTR *ppszHostDnsSuffix
    )
{
    DWORD dwError = 0;
    PSTR pszHostDnsSuffix = NULL;
    struct hostent* pHost = NULL;
    PCSTR pszDot = NULL;
    PCSTR pszFoundFqdn = NULL;
    PCSTR pszFoundDomain = NULL;

    pHost = gethostbyname(pszHostname);
    if (!pHost)
    {
        dwError = LWDNS_ERROR_NO_SUCH_ADDRESS;
        BAIL_ON_LWDNS_ERROR(dwError);
    }

    //
    // We look for the first name that looks like an FQDN.  This is
    // the same heuristics used by other software such as Kerberos and
    // Samba.
    //
    pszDot = strchr(pHost->h_name, '.');
    if (pszDot)
    {
        pszFoundFqdn = pHost->h_name;
        pszFoundDomain = pszDot + 1;
    }
    else
    {
        int i;
        for (i = 0; pHost->h_aliases[i]; i++)
        {
            pszDot = strchr(pHost->h_aliases[i], '.');
            if (pszDot)
            {
                pszFoundFqdn = pHost->h_aliases[i];
                pszFoundDomain = pszDot + 1;
                break;
            }
        }
    }

    if (!pszFoundDomain)
    {
        dwError = LWDNS_ERROR_NO_SUCH_ADDRESS;
        BAIL_ON_LWDNS_ERROR(dwError)
    }

    dwError = DNSAllocateString(pszFoundDomain, &pszHostDnsSuffix);
    BAIL_ON_LWDNS_ERROR(dwError);

cleanup:
    *ppszHostDnsSuffix = pszHostDnsSuffix;

    return dwError;

error:
    LWDNS_SAFE_FREE_STRING(pszHostDnsSuffix);

    goto cleanup;
}

static
DWORD
GetAllInterfaceAddresses(
    OUT PSOCKADDR_IN* ppAddressArray,
    OUT PDWORD pdwAddressCount
    )
{
    DWORD dwError = 0;
    PSOCKADDR_IN pAddressArray = NULL;
    DWORD dwAddressCount = 0;
    PLW_INTERFACE_INFO pInterfaceArray = NULL;
    DWORD dwInterfaceCount = 0;
    DWORD iAddr = 0;

    dwError = DNSGetNetworkInterfaces(
                    &pInterfaceArray,
                    &dwInterfaceCount);
    BAIL_ON_LWDNS_ERROR(dwError);

    if (!dwInterfaceCount)
    {
        dwError = LWDNS_ERROR_NO_INTERFACES;
        BAIL_ON_LWDNS_ERROR(dwError);
    }

    dwError = DNSAllocateMemory(
                    sizeof(SOCKADDR_IN) * dwInterfaceCount,
                    (PVOID*)&pAddressArray);
    BAIL_ON_LWDNS_ERROR(dwError);

    dwAddressCount = dwInterfaceCount;

    for (iAddr = 0; iAddr < dwAddressCount; iAddr++)
    {
        PSOCKADDR_IN pSockAddr = &pAddressArray[iAddr];
        PLW_INTERFACE_INFO pInterfaceInfo = &pInterfaceArray[iAddr];

        pSockAddr->sin_family = pInterfaceInfo->ipAddr.sa_family;
        pSockAddr->sin_addr = ((PSOCKADDR_IN)&pInterfaceInfo->ipAddr)->sin_addr;
    }

cleanup:
    if (pInterfaceArray)
    {
        DNSFreeNetworkInterfaces(
                pInterfaceArray,
                dwInterfaceCount);
    }

    *ppAddressArray = pAddressArray;
    *pdwAddressCount = dwAddressCount;

    return dwError;

error:
    LWDNS_SAFE_FREE_MEMORY(pAddressArray);
    dwAddressCount = 0;

    goto cleanup;
}

static
DWORD
AllocateStringFromWC16String(
    OUT PSTR* ppszOutputString,
    IN PCWSTR pwszInputString
    )
{
    DWORD dwError = 0;
    PSTR pszResult = NULL;

    pszResult = awc16stombs(pwszInputString);
    if (!pszResult)
    {
        dwError = LWDNS_ERROR_STRING_CONV_FAILED;
    }

    *ppszOutputString = pszResult;

    return dwError;
}

static
VOID
FreeStringFromWC16String(
    IN OUT PSTR* ppszAllocatedString
    )
{
    PSTR pszString = *ppszAllocatedString;
    if (pszString)
    {
        free(pszString);
        *ppszAllocatedString = NULL;
    }
}

static
DWORD
SetupCredentials(
    IN PCSTR pszHostname,
    OUT PSTR* ppszHostDnsSuffix
    )
{
    DWORD dwError = 0;
    PSTR pszHostDnsSuffixResult = NULL;
    PSTR pszHostDnsSuffix = NULL;
    PSTR pszMachineAccountName = NULL;
    PSTR pszDnsDomainName = NULL;
    HANDLE hPasswordStore = NULL;
    PLWPS_PASSWORD_INFO pPasswordInfo = NULL;

    dwError = LwpsOpenPasswordStore(
                  LWPS_PASSWORD_STORE_DEFAULT,
                  &hPasswordStore);
    BAIL_ON_LWDNS_ERROR(dwError);

    dwError = LwpsGetPasswordByHostName(
                  hPasswordStore,
                  pszHostname,
                  &pPasswordInfo);
    BAIL_ON_LWDNS_ERROR(dwError);

    dwError = AllocateStringFromWC16String(
                    &pszHostDnsSuffix,
                    pPasswordInfo->pwszHostDnsDomain);
    BAIL_ON_LWDNS_ERROR(dwError);

    dwError = AllocateStringFromWC16String(
                    &pszMachineAccountName,
                    pPasswordInfo->pwszMachineAccount);
    BAIL_ON_LWDNS_ERROR(dwError);

    dwError = AllocateStringFromWC16String(
                    &pszDnsDomainName,
                    pPasswordInfo->pwszDnsDomainName);
    BAIL_ON_LWDNS_ERROR(dwError);

    dwError = DNSKrb5Init(pszMachineAccountName, pszDnsDomainName);
    BAIL_ON_LWDNS_ERROR(dwError);

    dwError = DNSAllocateString(
                    pszHostDnsSuffix,
                    &pszHostDnsSuffixResult);
    BAIL_ON_LWDNS_ERROR(dwError);

cleanup:
    FreeStringFromWC16String(&pszHostDnsSuffix);
    FreeStringFromWC16String(&pszMachineAccountName);
    FreeStringFromWC16String(&pszDnsDomainName);

    if (pPasswordInfo)
    {
        LwpsFreePasswordInfo(hPasswordStore, pPasswordInfo);
    }

    if (hPasswordStore)
    {
        LwpsClosePasswordStore(hPasswordStore);
    }

    *ppszHostDnsSuffix = pszHostDnsSuffixResult;

    return dwError;

error:
    LWDNS_SAFE_FREE_STRING(pszHostDnsSuffixResult);

    goto cleanup;
}

static
VOID
CleanupCredentials(
    VOID
    )
{
    DNSKrb5Shutdown();
}

static
VOID
LogMessage(
    LWDNSLogLevel logLevel,
    PCSTR         pszFormat,
    va_list       msgList
    )
{
    PCSTR pszEntryType = NULL;
    time_t currentTime;
    struct tm tmp = {0};
    char timeBuf[128];

    switch (logLevel)
    {
        case LWDNS_LOG_LEVEL_ALWAYS:
        {
            pszEntryType = LWDNS_INFO_TAG;
            break;
        }
        case LWDNS_LOG_LEVEL_ERROR:
        {
            pszEntryType = LWDNS_ERROR_TAG;
            break;
        }

        case LWDNS_LOG_LEVEL_WARNING:
        {
            pszEntryType = LWDNS_WARN_TAG;
            break;
        }

        case LWDNS_LOG_LEVEL_INFO:
        {
            pszEntryType = LWDNS_INFO_TAG;
            break;
        }

        case LWDNS_LOG_LEVEL_VERBOSE:
        {
            pszEntryType = LWDNS_VERBOSE_TAG;
            break;
        }

        case LWDNS_LOG_LEVEL_DEBUG:
        {
            pszEntryType = LWDNS_DEBUG_TAG;
            break;
        }

        default:
        {
            pszEntryType = LWDNS_VERBOSE_TAG;
            break;
        }
    }

    currentTime = time(NULL);
    localtime_r(&currentTime, &tmp);

    strftime(timeBuf, sizeof(timeBuf), LWDNS_LOG_TIME_FORMAT, &tmp);

    fprintf(stdout, "%s:%s:", timeBuf, pszEntryType);
    vfprintf(stdout, pszFormat, msgList);
    fprintf(stdout, "\n");
    fflush(stdout);
}

static
VOID
ShowUsage(
    PCSTR pszProgramName
    )
{
    fprintf(stdout,
            "Usage: %s [options]\n"
            "\n"
            "    Registers IP addresses and corresponding PTR records in DNS via a"
            "    secure dynamic DNS update.  By default, will register all interface"
            "    addresses using the default FQDN as determined by the machine"
            "    password store or the canonical hostname returned by "
            "    gethostbyname(gethostname()) if --nocreds is used.\n"
            "\n"
            "  where options can be:\n"
            "\n"
            "    --loglevel LEVEL -- Sets log level, where LEVEL can be one of\n"
            "                        error, warning, info, verbose, debug.\n"
            "\n"
            "    --ipaddress IP   -- Sets IP address to register for this computer.\n"
            "                        This can be specified multiple times.\n"
            "\n"
            "    --fqdn FQDN      -- FQDN to register.\n"
            "\n"
            "    --nocreds        -- Do not use domain credentials.\n"
            "\n"
            "    --show           -- Show IP addresses and FQDN used.\n"
            "\n"
            "  examples:\n"
            "\n"
            "    %s --ipaddress 192.168.186.129\n"
            "    %s --ipaddress 192.168.186.129 --fqdn joe.example.com\n"
            "", pszProgramName, pszProgramName, pszProgramName);
}

static
VOID
PrintError(
    IN DWORD dwError
    )
{
    DWORD dwErrorBufferSize = 0;
    BOOLEAN bPrintOrigError = TRUE;

    dwErrorBufferSize = DNSGetErrorString(dwError, NULL, 0);

    if (dwErrorBufferSize > 0)
    {
        DWORD dwError2 = 0;
        PSTR pszErrorBuffer = NULL;

        dwError2 = DNSAllocateMemory(
                    dwErrorBufferSize,
                    (PVOID*)&pszErrorBuffer);

        if (!dwError2)
        {
            DWORD dwLen = DNSGetErrorString(dwError, pszErrorBuffer, dwErrorBufferSize);

            if ((dwLen == dwErrorBufferSize) && !IsNullOrEmptyString(pszErrorBuffer))
            {
                fprintf(stderr, "Failed to update DNS.  %s\n", pszErrorBuffer);
                bPrintOrigError = FALSE;
            }
        }

        LWDNS_SAFE_FREE_STRING(pszErrorBuffer);
    }

    if (bPrintOrigError)
    {
        fprintf(stderr, "Failed to update DNS. Error code [%d]\n", dwError);
    }
}

