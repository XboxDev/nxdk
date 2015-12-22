// ******************************************************************
// *
// *    .,-:::::    .,::      .::::::::.    .,::      .:
// *  ,;;;'````'    `;;;,  .,;;  ;;;'';;'   `;;;,  .,;; 
// *  [[[             '[[,,[['   [[[__[[\.    '[[,,[['  
// *  $$$              Y$$$P     $$""""Y$$     Y$$$P    
// *  `88bo,__,o,    oP"``"Yo,  _88o,,od8P   oP"``"Yo,  
// *    "YUMMMMMP",m"       "Mm,""YUMMMP" ,m"       "Mm,
// *
// *   Cxbx->Core->Exe.cpp
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

#include <stdio.h>
#include <memory.h>

// construct via Exe file
Exe::Exe(const char *x_szFilename)
{
    ConstructorInit();

    printf("Exe::Exe: Opening Exe file...");

    FILE *ExeFile = fopen(x_szFilename, "rb");

    // verify Exe file was opened
    if(ExeFile == 0)
    {
        SetError("Could not open Exe file.", true);
        return;
    }

    printf("OK\n");

    // ignore dos stub (if exists)
    {
        printf("Exe::Exe: Reading DOS stub...");

        if(fread(&m_DOSHeader.m_magic, sizeof(m_DOSHeader.m_magic), 1, ExeFile) != 1)
        {
            SetError("Unexpected read error while reading magic number", true);
            goto cleanup;
        }

        if(m_DOSHeader.m_magic == *(uint16*)"MZ")
        {
            printf("Found, Ignoring...");

            if(fread(&m_DOSHeader.m_cblp, sizeof(m_DOSHeader)-2, 1, ExeFile) != 1)
            {
                SetError("Unexpected read error while reading DOS stub", true);
                goto cleanup;
            }

            fseek(ExeFile, m_DOSHeader.m_lfanew, SEEK_SET);

            printf("OK\n");
        }
        else
        {
            printf("None (OK)\n");
        }
    }

    // read PE header
    {
        printf("Exe::Exe: Reading PE header...");

        if(fread(&m_Header, sizeof(m_Header), 1, ExeFile) != 1)
        {
            SetError("Unexpected read error while reading PE header", true);
            goto cleanup;
        }

        if(m_Header.m_magic != *(uint32*)"PE\0\0")
        {
            SetError("Invalid file (could not locate PE header)", true);
            goto cleanup;
        }

        printf("OK\n");
    }

    // read optional header
    {
        printf("Exe::Exe: Reading Optional Header...");

        if(fread(&m_OptionalHeader, sizeof(m_OptionalHeader), 1, ExeFile) != 1)
        {
            SetError("Unexpected read error while reading PE optional header", true);
            goto cleanup;
        }

        if(m_OptionalHeader.m_magic != 0x010B)
        {
            SetError("Invalid file (could not locate PE optional header)", true);
            goto cleanup;
        }

         printf("OK\n");
    }

    // read section headers
    {
        m_SectionHeader = new SectionHeader[m_Header.m_sections];

        printf("Exe::Exe: Reading Section Headers...\n");

        for(uint32 v=0;v<m_Header.m_sections;v++)
        {
            printf("Exe::Exe: Reading Section Header 0x%.04X...", v);

            if(fread(&m_SectionHeader[v], sizeof(SectionHeader), 1, ExeFile) != 1)
            {
                char buffer[255];
                sprintf(buffer, "Could not read PE section header %d (%Xh)", v, v);
                SetError(buffer, true);
                goto cleanup;
            }

            printf("OK %d\n", v);
        }
    }

    // read sections
    {
        printf("Exe::Exe: Reading Sections...\n");

        m_bzSection = new uint08*[m_Header.m_sections];

        for(uint32 v=0;v<m_Header.m_sections;v++)
        {
            printf("Exe::Exe: Reading Section 0x%.04X...", v);

            uint32 raw_size = m_SectionHeader[v].m_sizeof_raw;
            uint32 raw_addr = m_SectionHeader[v].m_raw_addr;

            m_bzSection[v] = new uint08[raw_size];

            memset(m_bzSection[v], 0, raw_size);

            if(raw_size == 0)
            {
                printf("OK\n");
                continue;
            }

            // read current section from file (if raw_size > 0)
            {
                fseek(ExeFile, raw_addr, SEEK_SET);

                if(fread(m_bzSection[v], raw_size, 1, ExeFile) != 1)
                {
                    char buffer[255];
                    sprintf(buffer, "Could not read PE section %d (%Xh)", v, v);
                    SetError(buffer, true);
                    goto cleanup;
                }
            }

            printf("OK\n");
        }
    }

    printf("Exe::Exe: Exe %s was successfully opened.\n", x_szFilename);

cleanup:

    if(GetError() != 0)
    {
        printf("FAILED!\n");
        printf("Exe::Exe: ERROR -> %s\n", GetError());
    }

    fclose(ExeFile);
}

// constructor initialization
void Exe::ConstructorInit()
{
    m_SectionHeader = 0;
    m_bzSection     = 0;
}

// deconstructor
Exe::~Exe()
{
    if(m_bzSection != 0)
    {
        for(uint32 v=0;v<m_Header.m_sections;v++)
            delete[] m_bzSection[v];

        delete[] m_bzSection;
    }

    delete[] m_SectionHeader;
}

// export to Exe file
void Exe::Export(const char *x_szExeFilename)
{
    if(GetError() != 0)
        return;

    printf("Exe::Export: Opening Exe file...");

    FILE *ExeFile = fopen(x_szExeFilename, "wb");

    // verify file was opened successfully
    if(ExeFile == 0)
    {
        SetError("Could not open .exe file.", false);
        return;
    }

    printf("OK\n");

    // write dos stub
    {
        printf("Exe::Export: Writing DOS stub...");

        if(fwrite(bzDOSStub, sizeof(bzDOSStub), 1, ExeFile) != 1)
        {
            SetError("Could not write dos stub", false);
            goto cleanup;
        }

        printf("OK\n");
    }

    // write pe header
    {
        printf("Exe::Export: Writing PE Header...");

        if(fwrite(&m_Header, sizeof(Header), 1, ExeFile) != 1)
        {
            SetError("Could not write PE header", false);
            goto cleanup;
        }

        printf("OK\n");
    }

    // write optional header
    {
        printf("Exe::Export: Writing Optional Header...");

        if(fwrite(&m_OptionalHeader, sizeof(OptionalHeader), 1, ExeFile) != 1)
        {
            SetError("Could not write PE optional header", false);
            goto cleanup;
        }

        printf("OK\n");
    }

    // write section header
    {
        printf("Exe::Export: Writing Section Headers...\n");

        for(uint32 v=0;v<m_Header.m_sections;v++)
        {
            printf("Exe::Export: Writing Section Header 0x%.04X...", v);

            if(fwrite(&m_SectionHeader[v], sizeof(SectionHeader), 1, ExeFile) != 1)
            {
                char buffer[255];
                sprintf(buffer, "Could not write PE section header %d (%Xh)", v, v);
                SetError(buffer, false);
                goto cleanup;
            }

            printf("OK\n");
        }
    }

    // write sections
    {
        printf("Exe::Export: Writing Sections...\n");

        for(uint32 v=0;v<m_Header.m_sections;v++)
        {
            printf("Exe::Export: Writing Section 0x%.04X...", v);

            uint32 RawSize = m_SectionHeader[v].m_sizeof_raw;
            uint32 RawAddr = m_SectionHeader[v].m_raw_addr;

            fseek(ExeFile, RawAddr, SEEK_SET);

            if(RawSize == 0)
            {
                printf("OK\n");
                continue;
            }

            if(fwrite(m_bzSection[v], RawSize, 1, ExeFile) != 1)
            {
                char buffer[255];
                sprintf(buffer, "Could not write PE section %d (%Xh)", v, v);
                SetError(buffer, false);
                goto cleanup;
            }

            fflush(ExeFile);

            printf("OK\n");
        }
    }

cleanup:

    if(GetError() != 0)
    {
        printf("FAILED!\n");
        printf("Exe::Export: ERROR -> %s\n", GetError());
    }

    fclose(ExeFile);

    return;
}

// return a modifiable pointer inside this structure that corresponds to a virtual address
uint08 *Exe::GetAddr(uint32 x_dwVirtualAddress)
{
    for(uint32 v=0;v<m_Header.m_sections;v++)
    {
        uint32 virt_addr = m_SectionHeader[v].m_virtual_addr;
        uint32 virt_size = m_SectionHeader[v].m_virtual_size;

        if( (x_dwVirtualAddress >= virt_addr) && (x_dwVirtualAddress < (virt_addr + virt_size)) )
            return &m_bzSection[v][x_dwVirtualAddress - virt_addr];
    }

    return 0;
}
