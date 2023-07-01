// This file is part of Cxbe
// SPDX-License-Identifier: GPL-2.0-or-later

// SPDX-FileCopyrightText: 2023 Erik Abair

#include "Xbe.h"
#include "Common.h"

#include <cstring>

// program entry point
int main(int argc, char *argv[])
{
    char szErrorMessage[ERROR_LEN+1]  = {0};
    char szXbeFilename[OPTION_LEN+1]  = {0};
    char szMode[OPTION_LEN+1]         = "retail";
    bool bRetail;

    const char *program = argv[0];
    const char *program_desc = "XBE header dumper (Version: " VERSION ")";
    Option options[] = {
        { szXbeFilename,  NULL,       "xbefile"        },
        { szMode,         "MODE",     "{debug|retail}" },
        { NULL }
    };

    if(ParseOptions(argv, argc, options, szErrorMessage)) {
        goto cleanup;
    }

    if(CompareString(szMode, "RETAIL"))
        bRetail = true;
    else if(CompareString(szMode, "DEBUG"))
        bRetail = false;
    else
    {
        strncpy(szErrorMessage, "invalid MODE", ERROR_LEN);
        goto cleanup;
    }

    // verify we received the required parameters
    if(szXbeFilename[0] == '\0')
    {
        ShowUsage(program, program_desc, options);
        return 1;
    }

    {
        Xbe xbe_file(szXbeFilename);
        if (xbe_file.GetError()) {
            strncpy(szErrorMessage, xbe_file.GetError(), ERROR_LEN);
            goto cleanup;
        }

        xbe_file.DumpInformation(stdout);
    }

cleanup:

    if(szErrorMessage[0] != 0)
    {
        ShowUsage(program, program_desc, options);

        printf("\n");
        printf(" *  Error : %s\n", szErrorMessage);

        return 1;
    }

    return 0;
}
