// This file is part of Cxbe
// SPDX-License-Identifier: GPL-2.0-or-later

// SPDX-FileCopyrightText: 2002-2003 Aaron Robinson <caustik@caustik.com>
// SPDX-FileCopyrightText: 2019 Jannik Vogel
// SPDX-FileCopyrightText: 2021 Stefan Schmidt

#include "Common.h"
#include "Exe.h"
#include "Xbe.h"

#include <string.h>

// program entry point
int main(int argc, char *argv[])
{
    char szErrorMessage[ERROR_LEN + 1] = { 0 };
    char szExeFilename[OPTION_LEN + 1] = { 0 };
    char szXbeFilename[OPTION_LEN + 1] = { 0 };
    char szDumpFilename[OPTION_LEN + 1] = { 0 };
    char szXbeTitle[OPTION_LEN + 1] = "Untitled";
    char szXbeTitleID[OPTION_LEN + 1] = "";
    char szMode[OPTION_LEN + 1] = "retail";
    char szLogo[OPTION_LEN + 1] = "";

    bool bRetail;
    uint32 dwTitleId = 0x4358270F; // CX-9999

    const char *program = argv[0];
    const char *program_desc = "CXBE EXE to XBE (win32 to Xbox) Relinker (Version: " VERSION ")";
    Option options[] = { { szExeFilename, NULL, "exefile" },
                         { szXbeFilename, "OUT", "filename" },
                         { szDumpFilename, "DUMPINFO", "filename" },
                         { szXbeTitle, "TITLE", "title" },
                         { szXbeTitleID, "TITLEID", "{%c%c-%u|%x}" },
                         { szMode, "MODE", "{debug|retail}" },
                         { szLogo, "LOGO", "filename" },
                         { NULL } };

    if(ParseOptions(argv, argc, options, szErrorMessage))
    {
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

    if(strlen(szXbeTitle) > 40)
    {
        printf("WARNING: Title too long, trimming\n");
        szXbeTitle[40] = '\0';
    }

    // interpret title id
    if(szXbeTitleID[0])
    {
        bool hex = true;
        for(int i = 0; szXbeTitleID[i]; ++i)
        {
            if(szXbeTitleID[i] == '-')
            {
                hex = false;
                break;
            }
        }
        if(hex)
        {
            sscanf(szXbeTitleID, "%x", &dwTitleId);
        }
        else
        {
            char titlechar[2] = { 'C', 'X' };
            unsigned titleno = 9999;
            sscanf(szXbeTitleID, "%c%c-%u", &titlechar[0], &titlechar[1], &titleno);
            if(titleno > 0xFFFF)
            {
                printf("WARNING: Title ID number too high (max is 65535)\n");
                titleno = 0xFFFF;
            }
            dwTitleId = (titlechar[0] << 24) | (titlechar[1] << 16) | titleno;
        }
    }

    // verify we received the required parameters
    if(szExeFilename[0] == '\0')
    {
        ShowUsage(program, program_desc, options);
        return 1;
    }

    // if we don't have an Xbe filename, generate one from szExeFilename
    if(szXbeFilename[0] == '\0')
    {
        if(GenerateFilename(szXbeFilename, ".xbe", szExeFilename, ".exe"))
        {
            strncpy(szErrorMessage, "Unable to generate Exe Path", ERROR_LEN);
            goto cleanup;
        }
    }

    // open and convert Exe file
    {
        Exe *ExeFile = new Exe(szExeFilename);

        if(ExeFile->GetError() != 0)
        {
            strncpy(szErrorMessage, ExeFile->GetError(), ERROR_LEN);
            goto cleanup;
        }

        std::vector<uint08> logo;
        std::vector<uint08> *LogoPtr = nullptr;
        if(szLogo[0] != '\0')
        {
            logo = pgmToLogoBitmap(szLogo);
            logo = Xbe::ImageToLogoBitmap(logo);
            LogoPtr = &logo;
        }

        Xbe *XbeFile = new Xbe(ExeFile, szXbeTitle, dwTitleId, bRetail, LogoPtr);

        if(XbeFile->GetError() != 0)
        {
            strncpy(szErrorMessage, XbeFile->GetError(), ERROR_LEN);
            goto cleanup;
        }

        if(szDumpFilename[0] != 0)
        {
            FILE *outfile = fopen(szDumpFilename, "wt");
            XbeFile->DumpInformation(outfile);
            fclose(outfile);

            if(XbeFile->GetError() != 0)
            {
                if(XbeFile->IsFatal())
                {
                    strncpy(szErrorMessage, XbeFile->GetError(), ERROR_LEN);
                    goto cleanup;
                }
                else
                {
                    printf("DUMPINFO -> Warning: %s\n", XbeFile->GetError());
                    XbeFile->ClearError();
                }
            }
        }

        XbeFile->Export(szXbeFilename);

        if(XbeFile->GetError() != 0)
        {
            strncpy(szErrorMessage, XbeFile->GetError(), ERROR_LEN);
            goto cleanup;
        }
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
