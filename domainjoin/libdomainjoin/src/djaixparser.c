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

#include "domainjoin.h"
#include "ctfileutils.h"
#include "djaixparser.h"

#define GCE(x) GOTO_CLEANUP_ON_CENTERROR((x))

ssize_t
DJFindStanza(const DynamicArray *lines, PCSTR name)
{
    ssize_t i;
    size_t namelen = strlen(name);

    for(i = 0; i < lines->size; i++)
    {
        PCSTR line = *(PCSTR *)CTArrayGetItem((DynamicArray*)lines, i,
                sizeof(PCSTR));
        while (*line != '\0' && isspace(*line))
            line++;

        if(!strncmp(line, name, namelen) && line[namelen] == ':')
            return i;
    }
    return -1;
}

ssize_t DJFindLine(const DynamicArray *lines, const char *stanza, const char *name)
{
    ssize_t i = DJFindStanza(lines, stanza);
    
    if(i == -1)
        return -1;

    for(; i < lines->size; i++)
    {
        PCSTR line = *(PCSTR *)CTArrayGetItem((DynamicArray*)lines, i,
                sizeof(PCSTR));

        while (*line != '\0' && isspace(*line))
            line++;

        if(strncmp(line, name, strlen(name)))
            continue;
        line += strlen(name);
       
        while (*line != '\0' && isspace(*line))
            line++;

        if(*line != '=')
            continue;

        return i;
    }

    return -1;
}

CENTERROR DJGetOptionValue(const DynamicArray *lines, PCSTR stanza, PCSTR name, PSTR *value)
{
    CENTERROR ceError = CENTERROR_SUCCESS;
    ssize_t i = DJFindLine(lines, stanza, name);
    PCSTR line;
    PSTR _value = NULL;

    *value = NULL;
    
    if(i == -1)
        GCE(ceError = CENTERROR_CFG_VALUE_NOT_FOUND);

    line = *(PCSTR *)CTArrayGetItem((DynamicArray*)lines, i,
            sizeof(PCSTR));

    while (*line != '\0' && isspace(*line))
        line++;

    line += strlen(name);
   
    while (*line != '\0' && isspace(*line))
        line++;

    if(*line != '=')
    {
        GCE(ceError = CENTERROR_CFG_INVALID_SIGNATURE);
    }
    line++;

    GCE(ceError = CTStrdup(line, &_value));
    CTStripWhitespace(_value);
    /* Remove the quotes around the value, if they exist */
    if(CTStrStartsWith(_value, "\"") && CTStrEndsWith(_value, "\""))
    {
        size_t len = strlen(_value);
        memmove(_value, _value + 1, len - 2 );
        _value[len - 2 ] = '\0';
    }

    *value = _value;
    _value = NULL;

cleanup:
    CT_SAFE_FREE_STRING(_value);
    return ceError;
}

CENTERROR
DJSetOptionValue(DynamicArray *lines, PCSTR stanza, PCSTR name, PCSTR value)
{
    ssize_t index;
    PSTR newLine = NULL;
    CENTERROR ceError = CENTERROR_SUCCESS;

    if(strchr(value, ' '))
    {
        /* Fixes bug 5564 */
        GCE(ceError = CTAllocateStringPrintf(&newLine, "\t%s = \"%s\"\n", name, value));
    }
    else
        GCE(ceError = CTAllocateStringPrintf(&newLine, "\t%s = %s\n", name, value));

    index = DJFindLine(lines, stanza, name);

    if(index == -1)
    {
        index = DJFindStanza(lines, stanza);
        if(index == -1)
            GCE(ceError = CENTERROR_INVALID_OPERATION);
        index++;
    }
    else
    {
        CT_SAFE_FREE_STRING(*(PSTR *)CTArrayGetItem((DynamicArray*)lines,
                    index, sizeof(PSTR)));
        GCE(ceError = CTArrayRemove((DynamicArray *)lines, index,
                    sizeof(PSTR), 1));
    }

    GCE(ceError = CTArrayInsert((DynamicArray *)lines, index,
                sizeof(PSTR), &newLine, 1));

cleanup:
    return ceError;
}
