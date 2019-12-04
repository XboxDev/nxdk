// ******************************************************************
// *
// *    .,-:::::    .,::      .::::::::.    .,::      .:
// *  ,;;;'````'    `;;;,  .,;;  ;;;'';;'   `;;;,  .,;; 
// *  [[[             '[[,,[['   [[[__[[\.    '[[,,[['  
// *  $$$              Y$$$P     $$""""Y$$     Y$$$P    
// *  `88bo,__,o,    oP"``"Yo,  _88o,,od8P   oP"``"Yo,  
// *    "YUMMMMMP",m"       "Mm,""YUMMMP" ,m"       "Mm,
// *
// *   Cxbx->Standard->Cxbe->Main.cpp
// *
// *  This file is part of the Cxbx project.
// *
// *  Cxbx and Cxbe are free software; you can redistribute them
// *  and/or modify them under the terms of the GNU General Public
// *  License as published by the Free Software Foundation; either
// *  version 2 of the license, or (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// *  GNU General Public License for more details.
// *
// *  You should have recieved a copy of the GNU General Public License
// *  along with this program; see the file COPYING.
// *  If not, write to the Free Software Foundation, Inc.,
// *  59 Temple Place - Suite 330, Bostom, MA 02111-1307, USA.
// *
// *  (c) 2002-2003 Aaron Robinson <caustik@caustik.com>
// *
// *  All rights reserved
// *
// ******************************************************************
#include "Exe.h"
#include "Xbe.h"

#include <string.h>

// static global(s)
static void ShowUsage();
static void MakeUpper(char *str);

// program entry point
int main(int argc, char *argv[])
{
    char szErrorMessage[266] = {0};
    char szExeFilename[266]  = {0};
    char szXbeFilename[266]  = {0};
    char szDumpFilename[266] = {0};
    char szXbeTitle[256]     = "Untitled";
    bool bRetail             = true;

    // parse command line
    for(int v=1;v<argc;v++)
    {
        char *szOption    = 0;
        char *szParam     = 0;

        // if this isn't an option, it must be the Exe file
        if(argv[v][0] != '-')
        {
            strncpy(szExeFilename, argv[v], 265);
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
                strcpy(szErrorMessage, "Command line format error");
                goto cleanup;
            }

            argv[v][dwColon] = '\0';

            szOption = &argv[v][1];
            szParam  = &argv[v][dwColon + 1];
        }

        // interpret the current switch
        {
            char szOptionU[266] = {0};
            char szParamU[266] = {0};

            strncpy(szOptionU, szOption, 265);
            strncpy(szParamU, szParam, 265);

            MakeUpper(szOptionU);
            MakeUpper(szParamU);

            if(strcmp(szOptionU, "OUT") == 0)
            {
                strcpy(szXbeFilename, szParam);
            }
            else if(strcmp(szOptionU, "DUMPINFO") == 0)
            {
                strcpy(szDumpFilename, szParam);
            }
            else if(strcmp(szOptionU, "TITLE") == 0)
            {
                strcpy(szXbeTitle, szParam);
            }
            else if(strcmp(szOptionU, "MODE") == 0)
            {
                if(strcmp(szParamU, "RETAIL") == 0)
                    bRetail = true;
                else if(strcmp(szParamU, "DEBUG") == 0)
                    bRetail = false;
                else
                {
                    strcpy(szErrorMessage, "invalid MODE");
                    goto cleanup;
                }
            }
            else
            {
                char szBuffer[255];
                sprintf(szBuffer, "Unrecognized command : %s", szOption);
                strcpy(szErrorMessage, szBuffer);
                goto cleanup;
            }
        }
    }

    if(strlen(szXbeTitle) > 40)
    {
        printf("WARNING: Title too long, trimming\n");
        szXbeTitle[40] = '\0';
    }

    // verify we recieved the required parameters
    if(szExeFilename[0] == '\0')
    {
        ShowUsage();
        return 1;
    }

    // if we don't have an Xbe filename, generate one from szExeFilename
    if(szXbeFilename[0] == '\0')
    {
        strcpy(szXbeFilename, szExeFilename);

        char *szFilename = &szXbeFilename[0];

        // locate last \ or / (if there are any)
        {
            for(int c=0;szXbeFilename[c] != 0;c++)
                if(szXbeFilename[c] == '\\' || szXbeFilename[c] == '/')
                    szFilename = &szXbeFilename[c+1];
        }

        // locate and remove last . (if there are any)
        {
            char szWorkingU[266];

            char *szWorking = szFilename;

            for(int c=0;szFilename[c] != 0;c++)
                if(szFilename[c] == '.')
                    szWorking = &szFilename[c];

            strncpy(szWorkingU, szWorking, 265);
            MakeUpper(szWorkingU);

            if(strcmp(szWorkingU, ".EXE") == 0)
                strcpy(szWorking, ".xbe");
            else
                strcat(szXbeFilename, ".xbe");
        }
    }

    // open and convert Exe file
    {
        Exe *ExeFile = new Exe(szExeFilename);

        if(ExeFile->GetError() != 0)
        {
            strcpy(szErrorMessage, ExeFile->GetError());
            goto cleanup;
        }

        Xbe *XbeFile = new Xbe(ExeFile, szXbeTitle, bRetail);

        if(XbeFile->GetError() != 0)
        {
            strcpy(szErrorMessage, XbeFile->GetError());
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
                    strcpy(szErrorMessage, XbeFile->GetError());
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
            strcpy(szErrorMessage, XbeFile->GetError());
            goto cleanup;
        }
    }

cleanup:

    if(szErrorMessage[0] != 0)
    {
        ShowUsage();
    
        printf("\n");
        printf(" *  Error : %s\n", szErrorMessage);

        return 1;
    }

    return 0;
}

// show program usage
static void ShowUsage()
{
    printf
    (
        "CXBE EXE to XBE (win32 to Xbox) Relinker (Version: " VERSION ")\n"
        "\n" 
        "Usage : cxbe [options] [exefile]\n"
        "\n"
        "Options :\n"
        "\n"
        "  -OUT:filename\n"
        "  -DUMPINFO:filename\n"
        "  -TITLE:title\n"
        "  -MODE:{debug|retail}\n"
    );
}

// convert string to upper case
static void MakeUpper(char *str)
{
    while(*str != '\0')
    {
        if(*str >= 'a' && *str <= 'z')
            *str = *str - ('a' - 'A');

        str++;
    }
}
