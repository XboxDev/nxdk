// SPDX-License-Identifier: GPL-2.0-or-later

// SPDX-FileCopyrightText: 2002-2003 Aaron Robinson <caustik@caustik.com>
// SPDX-FileCopyrightText: 2019 Jannik Vogel

#include "Common.h"
#include "Cxbx.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

// parse command line
int ParseOptions(char *argv[], int argc, const Option *options, char *szErrorMessage)
{
    for(int v = 1; v < argc; v++)
    {
        const Option *option = NULL;
        char *szOption = 0;
        char *szParam = 0;

        // if this isn't an option, it must be the default option
        if(argv[v][0] != '-')
        {
            strncpy(options[0].value, argv[v], OPTION_LEN);
            continue;
        }

        // locate the colon and separate option / parameters
        {
            uint dwColon = (uint)-1;

            for(uint c = 1; argv[v][c] != 0; c++)
            {
                if(argv[v][c] == ':')
                {
                    dwColon = c;
                    break;
                }
            }

            if(dwColon == (uint)-1)
            {
                strncpy(szErrorMessage, "Command line format error", ERROR_LEN);
                return 1;
            }

            argv[v][dwColon] = '\0';

            szOption = &argv[v][1];
            szParam = &argv[v][dwColon + 1];
        }

        // interpret the current switch
        option = &options[1];
        while(option->value)
        {
            if(CompareString(szOption, option->key))
            {
                strncpy(option->value, szParam, OPTION_LEN);
                break;
            }

            option++;
        }

        if(!option->value)
        {
            snprintf(szErrorMessage, ERROR_LEN, "Unrecognized command : %s", szOption);
            return 1;
        }
    }

    return 0;
}

// show program usage
void ShowUsage(const char *program, const char *desc, const Option *options)
{
    printf("%s\n"
           "\n"
           "Usage : %s [options] [%s]\n",
           desc, program, options[0].desc);

    if(options[1].value != NULL)
    {
        const Option *option = NULL;
        printf("\n"
               "Options :\n"
               "\n");
        option = &options[1];
        while(option->value)
        {
            printf("  -%s:%s\n", option->key, option->desc);
            option++;
        }
    }
}

int GenerateFilename(char *szNewPath, const char *szNewExtension, const char *szOldPath,
                     const char *szOldExtension)
{
    strncpy(szNewPath, szOldPath, OPTION_LEN);

    char *szFilename = &szNewPath[0];

    // locate last \ or / (if there are any)
    {
        for(int c = 0; szNewPath[c] != 0; c++)
            if(szNewPath[c] == '\\' || szNewPath[c] == '/')
                szFilename = &szNewPath[c + 1];
    }

    // locate and remove last . (if there are any)
    {
        char *szWorking = szFilename;

        for(int c = 0; szFilename[c] != 0; c++)
            if(szFilename[c] == '.')
                szWorking = &szFilename[c];

        if(CompareString(szWorking, szOldExtension))
            *szWorking = '\0';

        sintptr freeLength = OPTION_LEN - strlen(szNewPath);
        if(freeLength < strlen(szNewExtension))
        {
            return 1;
        }

        strncat(szNewPath, szNewExtension, freeLength);
    }

    return 0;
}

// case-insensitive string compare
bool CompareString(const char *szA, const char *szB)
{
    while(*szA != '\0' && *szB != '\0')
        if(toupper(*szA++) != toupper(*szB++))
            return false;
    return *szA == *szB;
}
