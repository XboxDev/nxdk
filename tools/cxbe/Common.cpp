// Licensed under GPLv2 or (at your option) any later version.
// Copyright (C) 2019 Jannik Vogel
// Copyright (C) 2002-2003 Aaron Robinson <caustik@caustik.com>

#include "Common.h"
#include "Cxbx.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

// parse command line
int ParseOptions(char *argv[], int argc,
                 const Option *options, char *szErrorMessage)
{
    for(int v=1;v<argc;v++)
    {
        const Option *option = NULL;
        char *szOption       = 0;
        char *szParam        = 0;

        // if this isn't an option, it must be the default option
        if(argv[v][0] != '-')
        {
            strncpy(options[0].value, argv[v], OPTION_LEN);
            continue;
        }

        // locate the colon and seperate option / parameters
        {
            uint dwColon = (uint)-1;

            for(uint c=1;argv[v][c] != 0;c++)
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
            szParam  = &argv[v][dwColon + 1];
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

// case-insensitive string compare
bool CompareString(const char *szA, const char *szB)
{
    while(*szA != '\0' && *szB != '\0')
        if(toupper(*szA++) != toupper(*szB++))
            return false;
    return *szA == *szB;
}
