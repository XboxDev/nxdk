// This file is part of Cxbe
// SPDX-License-Identifier: GPL-2.0-or-later

// SPDX-FileCopyrightText: 2002-2003 Aaron Robinson <caustik@caustik.com>
// SPDX-FileCopyrightText: 2019 Jannik Vogel
// SPDX-FileCopyrightText: 2021 Stefan Schmidt

#include "Exe.h"
#include "Xbe.h"

#include <memory.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <cstdio>

// construct via Exe file
Exe::Exe(const char *x_szFilename)
{
    ConstructorInit();

    printf("Exe::Exe: Opening Exe file...");

    FILE *ExeFile = fopen(x_szFilename, "rb");

    // verify Exe file was opened successfully
    if(ExeFile == NULL)
    {
        SetError("Could not open Exe file", true);
        goto cleanup;
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

    if(ExeFile != NULL)
    {
        fclose(ExeFile);
        ExeFile = NULL;
    }

    return;
}

Exe::Exe(class Xbe *x_Xbe, const char *x_szTitle, bool x_bRetail) {
    
    ConstructorInit();

    time_t CurrentTime;

    time(&CurrentTime);

    printf("Exe::Exe: Pass 1 (Simple Pass)...");
    
    //pass 1
    {
        //standard Pe magic Number
        m_Header.m_magic = *(uint32 *)"PE\0\0";
        //always i386
        m_Header.m_machine= IMAGE_FILE_MACHINE_I386;
        //m_Header.dwBaseAddr = 0x00010000;
        //we want the same number of sections as our xbe file
        m_Header.m_sections= (uint16) x_Xbe->m_Header.dwSections;
        //magic number
        m_Header.m_characteristics = 0x010F; //ripped from EmuExe.cpp



        //copies of various same header values
        {
            m_OptionalHeader.m_sizeof_stack_reserve = x_Xbe->m_Header.dwPeStackCommit;
            m_OptionalHeader.m_sizeof_heap_reserve= x_Xbe->m_Header.dwPeHeapReserve;
            m_OptionalHeader.m_sizeof_heap_commit= x_Xbe->m_Header.dwPeHeapCommit;
            m_OptionalHeader.m_sizeof_image= x_Xbe-> m_Header.dwPeSizeofImage;
            //dwPeChecksum    Is this needed?
            m_Header.m_timedate=x_Xbe->m_Header.dwPeTimeDate;
            m_OptionalHeader.m_image_base= x_Xbe->m_Header.dwBaseAddr;

        }
        // optional header init
        {
            m_Header.m_sizeof_optional_header =sizeof(OptionalHeader);
            m_OptionalHeader.m_magic=0x010B;
            m_OptionalHeader.m_file_alignment= PE_FILE_ALIGN;
            m_OptionalHeader.m_section_alignment= PE_SEGM_ALIGN;
            m_OptionalHeader.m_sizeof_headers= sizeof (bzDOSStub)+ sizeof(m_Header);
            m_OptionalHeader.m_sizeof_headers += sizeof (m_OptionalHeader)+ sizeof(*m_SectionHeader)*m_Header.m_sections;
            m_OptionalHeader.m_sizeof_headers = RoundUp(m_OptionalHeader.m_sizeof_headers, PE_FILE_ALIGN);
            m_OptionalHeader.m_data_directories = 0x10;
        }

        printf("OK\n");
        printf("Exe::Exe: Pass 2 (Calculating Requirements)...");

        //pass 2
        {
            // make-room cursor
            uint32 mrc = m_OptionalHeader.m_image_base + sizeof(m_Header+m_OptionalHeader); // This seems more logical than just m_Header


        }
            
        
    }
    
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

    // verify Exe file was opened successfully
    if(ExeFile == NULL)
    {
        SetError("Could not open Exe file", true);
        goto cleanup;
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

    if(ExeFile != NULL)
    {
        fclose(ExeFile);
        ExeFile = NULL;
    }

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
