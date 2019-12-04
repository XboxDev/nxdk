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
#include "Common.h"

#include <string.h>

// static global(s)
static void ShowUsage();

// program entry point
int main(int argc, char *argv[])
{
    char szErrorMessage[ERROR_LEN+1]  = {0};
    char szExeFilename[OPTION_LEN+1]  = {0};
    char szXbeFilename[OPTION_LEN+1]  = {0};
    char szDumpFilename[OPTION_LEN+1] = {0};
    char szXbeTitle[OPTION_LEN+1]     = "Untitled";
    bool bRetail                      = true;

    // parse command line
    for(int v=1;v<argc;v++)
    {
        char *szOption    = 0;
        char *szParam     = 0;

        // if this isn't an option, it must be the Exe file
        if(argv[v][0] != '-')
        {
            strncpy(szExeFilename, argv[v], OPTION_LEN);
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
                goto cleanup;
            }

            argv[v][dwColon] = '\0';

            szOption = &argv[v][1];
            szParam  = &argv[v][dwColon + 1];
        }

        // interpret the current switch
        {
            if(CompareString(szOption, "OUT"))
            {
                strncpy(szXbeFilename, szParam, OPTION_LEN);
            }
            else if(CompareString(szOption, "DUMPINFO"))
            {
                strncpy(szDumpFilename, szParam, OPTION_LEN);
            }
            else if(CompareString(szOption, "TITLE"))
            {
                strncpy(szXbeTitle, szParam, OPTION_LEN);
            }
            else if(CompareString(szOption, "MODE"))
            {
                if(CompareString(szParam, "RETAIL"))
                    bRetail = true;
                else if(CompareString(szParam, "DEBUG"))
                    bRetail = false;
                else
                {
                    strncpy(szErrorMessage, "invalid MODE", ERROR_LEN);
                    goto cleanup;
                }
            }
            else
            {
                snprintf(szErrorMessage, ERROR_LEN, "Unrecognized command : %s", szOption);
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
        strncpy(szXbeFilename, szExeFilename, OPTION_LEN);

        char *szFilename = &szXbeFilename[0];

        // locate last \ or / (if there are any)
        {
            for(int c=0;szXbeFilename[c] != 0;c++)
                if(szXbeFilename[c] == '\\' || szXbeFilename[c] == '/')
                    szFilename = &szXbeFilename[c+1];
        }

        // locate and remove last . (if there are any)
        {
            char *szWorking = szFilename;

            for(int c=0;szFilename[c] != 0;c++)
                if(szFilename[c] == '.')
                    szWorking = &szFilename[c];

            if(CompareString(szWorking, ".EXE"))
                *szWorking = '\0';

            sintptr freeLength = OPTION_LEN - strlen(szXbeFilename);
            if(freeLength < strlen(".xbe"))
            {
                strncpy(szErrorMessage, "Exe Path too long", ERROR_LEN);
                goto cleanup;
            }

            strncat(szXbeFilename, ".xbe", freeLength);
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

        Xbe *XbeFile = new Xbe(ExeFile, szXbeTitle, bRetail);

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
