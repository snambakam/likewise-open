/* Editor Settings: expandtabs and use 4 spaces for indentation
* ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
* -*- mode: c, c-basic-offset: 4 -*- */

/*
 * Copyright (C) Centeris Corporation 2004-2007
 * Copyright (C) Likewise Software    2007-2008
 * All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; either version 2.1 of 
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this program.  If not, see 
 * <http://www.gnu.org/licenses/>.
 */

#ifndef __DJ_PARSEHOSTS_H__
#define __DJ_PARSEHOSTS_H__

typedef struct __HOSTFILEALIAS
{
    PSTR pszAlias;
    struct __HOSTFILEALIAS *pNext;
} HOSTFILEALIAS, *PHOSTFILEALIAS;

typedef struct __HOSTSFILEENTRY
{
    PSTR pszIpAddress;
    PSTR pszCanonicalName;
    PHOSTFILEALIAS pAliasList;
} HOSTSFILEENTRY, *PHOSTSFILEENTRY;

typedef struct __HOSTSFILELINE
{
    PHOSTSFILEENTRY pEntry;
    PSTR pszComment;
    BOOLEAN bModified;

    struct __HOSTSFILELINE *pNext;

} HOSTSFILELINE, *PHOSTSFILELINE;

CENTERROR
DJReplaceNameInHostsFile(
    const char *filename,
    PSTR oldShortHostname,
    PSTR oldFdqnHostname,
    PSTR shortHostname,
    PSTR dnsDomainName
    );

CENTERROR
DJReplaceHostnameInMemory(
    PHOSTSFILELINE pHostsFileLineList,
    PCSTR oldShortHostname,
    PCSTR oldFqdnHostname,
    PCSTR shortHostname,
    PCSTR dnsDomainName
    );

CENTERROR
DJParseHostsFile(
    const char *filename,
    PHOSTSFILELINE* ppHostsFileLineList
    );

BOOLEAN
DJHostsFileWasModified(
    PHOSTSFILELINE pHostFileLineList
    );

void
DJFreeHostsFileLineList(
    PHOSTSFILELINE pHostsLineList
    );

CENTERROR
DJCopyMissingHostsEntry(
        PCSTR destFile, PCSTR srcFile,
        PCSTR entryName1, PCSTR entryName2);

#endif /* __DJ_PARSEHOSTS_H__ */
