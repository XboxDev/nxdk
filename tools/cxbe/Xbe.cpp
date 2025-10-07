// This file is part of Cxbe
// SPDX-License-Identifier: GPL-2.0-or-later

// SPDX-FileCopyrightText: 2002-2003 Aaron Robinson <caustik@caustik.com>
// SPDX-FileCopyrightText: 2018 Lucas Jansson
// SPDX-FileCopyrightText: 2019-2021 Stefan Schmidt
// SPDX-FileCopyrightText: 2019 Jannik Vogel

#include "Xbe.h"
#include "Exe.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <locale.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>

static const char kKernelImageName[] = "xboxkrnl.exe";
static uint32 CountNonKernelImportTableEntries(class Exe *x_Exe, uint32_t *extra_bytes);

static size_t BasenameOffset(const std::string &path)
{
    size_t sep_offset = path.find_last_of("/\\");
    if(sep_offset == std::string::npos)
    {
        return 0;
    }

    return sep_offset + 1;
}

// construct via Exe file object
Xbe::Xbe(class Exe *x_Exe, const char *x_szTitle, uint32 x_dwTitleID, uint32 x_dwRegions,
         uint32 x_dwVersion, bool x_bRetail, const std::vector<uint08> *logo,
         const char *x_szDebugPath)
{
    ConstructorInit();

    time_t CurrentTime;

    time(&CurrentTime);

    printf("Xbe::Xbe: Pass 1 (Simple Pass)...");
    std::string debug_path = x_szDebugPath;

    // pass 1
    {
        // standard Xbe magic number
        m_Header.dwMagic = *(uint32 *)"XBEH";

        // nobody has the private key yet, so zero this out
        memset(m_Header.pbDigitalSignature, 0, 256);

        // we'll only allow 0x00010000 for now
        m_Header.dwBaseAddr = 0x00010000;

        // this is a constant value
        m_Header.dwSizeofImageHeader = sizeof(m_Header);

        // we'll have the same number of sections as the Exe
        m_Header.dwSections = x_Exe->m_Header.m_sections;

        // TODO: allow configuration
        {
            memset(&m_Header.dwInitFlags, 0, sizeof(m_Header.dwInitFlags));

            m_Header.dwInitFlags.bLimit64MB = true;
            m_Header.dwInitFlags.bDontSetupHarddisk = false;
            m_Header.dwInitFlags.bMountUtilityDrive = true;
        }

        // various PE copies
        {
            // StackCommit actually means StackReserve
            m_Header.dwPeStackCommit = x_Exe->m_OptionalHeader.m_sizeof_stack_reserve;
            m_Header.dwPeHeapReserve = x_Exe->m_OptionalHeader.m_sizeof_heap_reserve;
            m_Header.dwPeHeapCommit = x_Exe->m_OptionalHeader.m_sizeof_heap_commit;
            m_Header.dwPeSizeofImage = x_Exe->m_OptionalHeader.m_sizeof_image;
            m_Header.dwPeChecksum = 0x00000000;
            m_Header.dwPeTimeDate = x_Exe->m_Header.m_timedate;
        }

        // build time/date
        m_Header.dwTimeDate = CurrentTime;

        m_Header.dwNonKernelImportDirAddr = 0;

        // Need one or more library version, otherwise VerifierX on XDKs crashes
        m_Header.dwLibraryVersions = 1;

        // TODO: generate these values
        m_Header.dwKernelLibraryVersionAddr = 0;
        m_Header.dwXAPILibraryVersionAddr = 0;
    }

    printf("OK\n");

    printf("Xbe::Xbe: Pass 2 (Calculating Requirements)...");

    // pass 2
    uint32 non_kernel_import_table_bytes = 0;
    {
        // make-room cursor
        uint32 mrc = m_Header.dwBaseAddr + sizeof(m_Header);

        // make room for certificate
        {
            m_Header.dwCertificateAddr = mrc;

            mrc += sizeof(m_Certificate);
        }

        // make room for section headers
        {
            m_Header.dwSectionHeadersAddr = mrc;

            mrc += m_Header.dwSections * (sizeof(*m_SectionHeader));

            // make room for head/tail reference count words
            mrc += (m_Header.dwSections + 1) * 2;

            // make room for section names
            for(uint32 v = 0; v < m_Header.dwSections; v++)
            {
                uint32 s = 0;

                if(x_Exe->m_SectionHeader_longname[v].m_longname)
                {
                    while(s < 255 && x_Exe->m_SectionHeader_longname[v].m_longname[s] != '\0')
                        s++;
                }
                else
                {
                    while(s < 8 && x_Exe->m_SectionHeader[v].m_name[s] != '\0')
                        s++;
                }

                mrc += s + 1;
            }
        }

        // make room for non-kernel imports and a null terminator
        {
            uint32 name_bytes = 0;
            uint32 num_entries = CountNonKernelImportTableEntries(x_Exe, &name_bytes);
            if(num_entries)
            {
                if(x_bRetail)
                {
                    printf("Xbe::Xbe: Non-kernel imports detected in RETAIL mode. Ignoring...\n");
                }
                else
                {
                    non_kernel_import_table_bytes = mrc;
                    mrc = RoundUp(mrc, 0x04);
                    m_Header.dwNonKernelImportDirAddr = mrc;
                    mrc += (1 + num_entries) * sizeof(XBE_IMAGE_IMPORT_DESCRIPTOR);
                    mrc = RoundUp(mrc, 0x04);
                    m_ImportNameAddr = mrc;
                    mrc += name_bytes;
                    mrc = RoundUp(mrc, 0x04);
                    non_kernel_import_table_bytes = mrc - non_kernel_import_table_bytes;
                }
            }
        }

        // make room for library versions
        {
            if(m_Header.dwLibraryVersions == 0)
            {
                m_Header.dwLibraryVersionsAddr = 0;
            }
            else
            {
                m_Header.dwLibraryVersionsAddr = mrc;
                mrc += m_Header.dwLibraryVersions * static_cast<uint32>(sizeof(*m_LibraryVersion));
            }
        }

        // make room for debug path / debug file names
        {
            uint32 path_bytes = debug_path.size() + 1;
            size_t sep_offset = BasenameOffset(debug_path);
            uint32 filename_bytes = path_bytes - sep_offset;

            mrc = RoundUp(mrc, 0x04);
            m_Header.dwDebugUnicodeFilenameAddr = mrc;
            mrc = RoundUp(mrc + filename_bytes * 2, 0x04);

            m_Header.dwDebugPathnameAddr = mrc;
            m_Header.dwDebugFilenameAddr = m_Header.dwDebugPathnameAddr + sep_offset;
            mrc += path_bytes;
        }

        // make room for largest possible logo bitmap
        {
            mrc = RoundUp(mrc, 0x10);

            m_Header.dwLogoBitmapAddr = mrc;
            if(!logo)
            {
                m_Header.dwSizeofLogoBitmap = defaultXbeLogoSize;
            }
            else
            {
                m_Header.dwSizeofLogoBitmap = logo->size();
            }

            mrc += m_Header.dwSizeofLogoBitmap;
        }

        // update size of headers
        m_Header.dwSizeofHeaders = mrc - m_Header.dwBaseAddr;
    }

    printf("OK\n");

    printf("Xbe::Xbe: Pass 3 (Generating Xbe)...\n");

    // pass 3
    {
        m_Header.dwPeBaseAddr = m_Header.dwBaseAddr + RoundUp(m_Header.dwSizeofHeaders, 0x1000) -
                                x_Exe->m_SectionHeader[0].m_virtual_addr;

        // encode entry point
        {
            printf("Xbe::Xbe: Encoding %s Entry Point...", x_bRetail ? "Retail" : "Debug");

            uint32 ep = x_Exe->m_OptionalHeader.m_entry + m_Header.dwPeBaseAddr;

            if(x_bRetail)
                ep ^= XOR_EP_RETAIL;
            else
                ep ^= XOR_EP_DEBUG;

            m_Header.dwEntryAddr = ep;

            printf("OK (0x%.08X)\n", ep);
        }

        {
            printf("Xbe::Xbe: Relocating TLS directory...");

            uint32 tls_directory =
                x_Exe->m_OptionalHeader.m_image_data_directory[IMAGE_DIRECTORY_ENTRY_TLS]
                    .m_virtual_addr;
            if(!tls_directory)
                m_Header.dwTLSAddr = 0;
            else
                m_Header.dwTLSAddr = tls_directory + m_Header.dwPeBaseAddr;

            printf("OK (0x%.08X)\n", m_Header.dwTLSAddr);
        }

        // header write cursor
        uint32 hwc = m_Header.dwBaseAddr + sizeof(m_Header);

        // check if we need to store extra header bytes (we always will)
        if(m_Header.dwSizeofHeaders > sizeof(m_Header))
        {
            printf("Xbe::Xbe: Found Extra Header Bytes...");

            uint32 ExSize = RoundUp(m_Header.dwSizeofHeaders - sizeof(m_Header), 0x1000);

            m_HeaderEx = new char[ExSize];
            memset(m_HeaderEx, 0, ExSize);

            printf("OK\n");
        }

        // start a write buffer inside of m_HeaderEx
        char *szBuffer = m_HeaderEx;

        // write certificate
        {
            // certificate size is a constant
            m_Certificate.dwSize = sizeof(m_Certificate);

            m_Certificate.dwTimeDate = CurrentTime;

            m_Certificate.dwTitleId = x_dwTitleID;

            // title name
            memset(m_Certificate.wszTitleName, 0, sizeof(m_Certificate.wszTitleName));
            // mbstowcs(m_Certificate.wszTitleName, x_szTitle, 40);
            const char *p = x_szTitle;
            char *q = (char *)m_Certificate.wszTitleName;
            while(*p != 0x00)
            {
                *q++ = *p++;
                *q++ = 0x00;
            }

            // zero out alternate ids
            {
                for(uint32 c = 0; c < 0x10; c++)
                    m_Certificate.dwAlternateTitleId[c] = 0;
            }

            // for now we'll just allow any media you could want
            m_Certificate.dwAllowedMedia =
                XBEIMAGE_MEDIA_TYPE_HARD_DISK | XBEIMAGE_MEDIA_TYPE_DVD_CD |
                XBEIMAGE_MEDIA_TYPE_MEDIA_BOARD | XBEIMAGE_MEDIA_TYPE_NONSECURE_HARD_DISK |
                XBEIMAGE_MEDIA_TYPE_NONSECURE_MODE;

            m_Certificate.dwGameRegion = x_dwRegions;

            // TODO: allow configuration
            m_Certificate.dwGameRatings = 0xFFFFFFFF;

            // always disk 0, AFAIK
            m_Certificate.dwDiskNumber = 0;

            m_Certificate.dwVersion = x_dwVersion;

            // generate blank LAN, signature, and alternate signature keys
            {
                for(uint32 v = 0; v < 0x10; v++)
                    m_Certificate.bzLanKey[v] = m_Certificate.bzSignatureKey[v] = 0;

                for(uint32 x = 0; x < 0x10; x++)
                    for(uint32 y = 0; y < 0x10; y++)
                        m_Certificate.bzTitleAlternateSignatureKey[x][y] = 0;
            }

            // write certificate
            {
                memcpy(szBuffer, &m_Certificate, sizeof(m_Certificate));

                szBuffer += sizeof(m_Certificate);

                hwc += sizeof(m_Certificate);
            }
        }

        // generate ascii title from certificate title name
        setlocale(LC_ALL, "English");
        // wcstombs(m_szAsciiTitle, m_Certificate.wszTitleName, 40);
        char *c = m_szAsciiTitle;
        char *d = (char *)m_Certificate.wszTitleName;
        while((uint16 *)d < &m_Certificate.wszTitleName[40] && *d)
        {
            *c++ = *d++;
            d++;
        }
        *c = '\0';

        // write section headers / section names
        {
            m_szSectionName = new char[m_Header.dwSections][256];

            m_SectionHeader = new SectionHeader[m_Header.dwSections];

            uint32 SectionCursor = RoundUp(m_Header.dwSizeofHeaders, 0x1000);

            // head/tail reference count write buffer
            uint16 *htrc = (uint16 *)(szBuffer + m_Header.dwSections * sizeof(*m_SectionHeader));

            // section write buffer
            char *secn = (char *)((uintptr_t)htrc + (m_Header.dwSections + 1) * 2);

            // head/tail reference count write cursor
            uint32 hwc_htrc = hwc + m_Header.dwSections * sizeof(*m_SectionHeader);

            // section write cursor
            uint32 hwc_secn = hwc_htrc + (m_Header.dwSections + 1) * 2;

            printf("Xbe::Xbe: Generating Section Headers...\n");

            for(uint32 v = 0; v < m_Header.dwSections; v++)
            {
                printf("Xbe::Xbe: Generating Section Header %.04X...", v);

                uint32 characteristics = x_Exe->m_SectionHeader[v].m_characteristics;

                memset(&m_SectionHeader[v].dwFlags, 0, sizeof(m_SectionHeader->dwFlags));

                // check for $$XTIMAGE or $$XSIMAGE and set the correct flags
                if(x_Exe->m_SectionHeader_longname[v].m_longname &&
                   (!strcmp(x_Exe->m_SectionHeader_longname[v].m_longname, "$$XTIMAGE") ||
                    !strcmp(x_Exe->m_SectionHeader_longname[v].m_longname, "$$XSIMAGE")))
                {
                    m_SectionHeader[v].dwFlags.bInsertedFile = true;
                    m_SectionHeader[v].dwFlags.bHeadPageRO = true;
                    m_SectionHeader[v].dwFlags.bTailPageRO = true;
                }
                else
                {
                    if(characteristics & IMAGE_SCN_MEM_WRITE)
                        m_SectionHeader[v].dwFlags.bWritable = true;

                    if((characteristics & IMAGE_SCN_MEM_EXECUTE) ||
                       (characteristics & IMAGE_SCN_CNT_CODE))
                        m_SectionHeader[v].dwFlags.bExecutable = true;

                    char *name = (x_Exe->m_SectionHeader_longname[v].m_longname)
                                     ? x_Exe->m_SectionHeader_longname[v].m_longname
                                     : (char *)x_Exe->m_SectionHeader[v].m_name;
                    m_SectionHeader[v].dwFlags.bPreload =
                        (strcmp(name, ".debug") && strncmp(name, ".debug_", 7));
                }

                m_SectionHeader[v].dwVirtualAddr =
                    x_Exe->m_SectionHeader[v].m_virtual_addr + m_Header.dwPeBaseAddr;

                m_SectionHeader[v].dwRawAddr = SectionCursor;

                // calculate sizeof_raw by locating the last non-zero value in the raw section data
                {
                    uint32 r = x_Exe->m_SectionHeader[v].m_sizeof_raw;
                    if(r > 0)
                    {
                        r--;
                        while(r > 0)
                        {
                            if(x_Exe->m_bzSection[v][r--] != 0)
                                break;
                        }
                    }

                    // word aligned
                    m_SectionHeader[v].dwSizeofRaw = RoundUp(r + 2, 4);
                }

                // calculate virtual size
                if(v < m_Header.dwSections - 1)
                    m_SectionHeader[v].dwVirtualSize =
                        x_Exe->m_SectionHeader[v + 1].m_virtual_addr -
                        x_Exe->m_SectionHeader[v].m_virtual_addr;
                else {
                    m_SectionHeader[v].dwVirtualSize =
                        RoundUp(x_Exe->m_SectionHeader[v].m_virtual_size, 4);

                    // force virtual size to be at least as large as the raw size
                    m_SectionHeader[v].dwVirtualSize = std::max(m_SectionHeader[v].dwSizeofRaw,
                        m_SectionHeader[v].dwVirtualSize);
                }

                SectionCursor += RoundUp(m_SectionHeader[v].dwSizeofRaw, 0x1000);

                // head/tail reference count
                {
                    m_SectionHeader[v].dwHeadSharedRefCountAddr = hwc_htrc;
                    htrc[v] = 0;

                    hwc_htrc += 2;

                    m_SectionHeader[v].dwTailSharedRefCountAddr = hwc_htrc;
                    htrc[v + 1] = 0;
                }

                // section name
                {
                    uint32 s = 0;

                    memset(secn, 0, 8);

                    m_SectionHeader[v].dwSectionNameAddr = hwc_secn;
                    if(x_Exe->m_SectionHeader_longname[v].m_longname)
                    {
                        while(s < 255 && x_Exe->m_SectionHeader_longname[v].m_longname[s] != '\0')
                        {
                            m_szSectionName[v][s] = secn[s] = x_Exe->m_SectionHeader_longname[v].m_longname[s];
                            s++;
                        }
                    }
                    else
                    {
                        while(s < 8 && x_Exe->m_SectionHeader[v].m_name[s] != '\0')
                        {
                            m_szSectionName[v][s] = secn[s] = x_Exe->m_SectionHeader[v].m_name[s];
                            s++;
                        }
                    }

                    m_szSectionName[v][s] = '\0';

                    secn += s + 1;
                    hwc_secn += s + 1;
                }

                m_SectionHeader[v].dwSectionRefCount = 0;

                // write section digest (just zeros)
                memset(m_SectionHeader[v].bzSectionDigest, 0, 20);

                // write section header
                memcpy(szBuffer, &m_SectionHeader[v], sizeof(*m_SectionHeader));

                szBuffer += sizeof(*m_SectionHeader);

                printf("OK\n");
            }

            hwc = hwc_secn;
            szBuffer = m_HeaderEx + hwc - (m_Header.dwBaseAddr + sizeof(m_Header));
        }

        // Reserve space for the non-kernel import table.
        {
            szBuffer += non_kernel_import_table_bytes;
            hwc += non_kernel_import_table_bytes;
        }

        // Write (placeholder) library versions
        {
            m_LibraryVersion = new LibraryVersion[m_Header.dwLibraryVersions];

            for(uint32 v = 0; v < m_Header.dwLibraryVersions; ++v)
            {
                char tmp[9];

                snprintf(tmp, sizeof(tmp), "CXBE%d", v);

                for(uint32 c = 0; c < 8; ++c)
                {
                    m_LibraryVersion[v].szName[c] = tmp[c];
                }
                m_LibraryVersion[v].wMajorVersion = 0;
                m_LibraryVersion[v].wMinorVersion = 0;
                m_LibraryVersion[v].wBuildVersion = 0;
                m_LibraryVersion[v].dwFlags.QFEVersion = 0;
                m_LibraryVersion[v].dwFlags.Approved = 0;
                m_LibraryVersion[v].dwFlags.bDebugBuild = 0;

                // write library version
                memcpy(szBuffer, &m_LibraryVersion[v], sizeof(*m_LibraryVersion));

                szBuffer += sizeof(*m_LibraryVersion);
                hwc += sizeof(*m_LibraryVersion);
            }
        }

        // write debug path / debug file names
        {
            uint08 *debug_path_field = GetAddr(m_Header.dwDebugPathnameAddr);
            uint32 path_size_with_terminator = debug_path.size() + 1;
            memcpy(debug_path_field, debug_path.c_str(), path_size_with_terminator);

            uint08 *unicode_filename = GetAddr(m_Header.dwDebugUnicodeFilenameAddr);
            uint08 *filename = GetAddr(m_Header.dwDebugFilenameAddr);
            do
            {
                *unicode_filename++ = *filename++;
                *unicode_filename++ = 0;
            } while(*filename);
        }

        {
            printf("Xbe::Xbe: Generating Logo Bitmap...");

            uint08 *RawAddr = GetAddr(m_Header.dwLogoBitmapAddr);

            if(logo)
            {
                memcpy(RawAddr, logo->data(), logo->size());
                printf("OK (custom)\n");
            }
            else
            {
                memcpy(RawAddr, defaultXbeLogo, defaultXbeLogoSize);
                printf("OK (default)\n");
            }
        }

        // write sections
        {
            printf("Xbe::Xbe: Generating Sections...\n");

            m_bzSection = new uint08 *[m_Header.dwSections];

            memset(m_bzSection, 0, m_Header.dwSections);

            for(uint32 v = 0; v < m_Header.dwSections; v++)
            {
                printf("Xbe::Xbe: Generating Section %.04X...", v);

                uint32 RawSize = m_SectionHeader[v].dwSizeofRaw;
                uint32 VirtSize = m_SectionHeader[v].dwVirtualSize;
                uint32 maxSize = std::max(VirtSize, RawSize);

                m_bzSection[v] = new uint08[maxSize];

                memset(m_bzSection[v], 0, maxSize);

                memcpy(m_bzSection[v], x_Exe->m_bzSection[v], RawSize);

                printf("OK\n");
            }
        }

        // process kernel and debug thunk tables
        if(!ProcessImportTable(x_Exe, x_bRetail))
        {
            goto cleanup;
        }
    }

    printf("Xbe::Xbe: Pass 4 (Finalizing)...\n");

    // pass 4
    {
        m_Header.dwSizeofImage = m_SectionHeader[m_Header.dwSections - 1].dwVirtualAddr +
                                 m_SectionHeader[m_Header.dwSections - 1].dwVirtualSize -
                                 m_Header.dwBaseAddr;

        if(m_Header.dwSizeofImage >= 64 * 1024 * 1024)
        {
            fprintf(
                stderr,
                "Xbe::Xbe: Image size exceeds 64MiB - this xbe will not run on retail systems\n");
        }
        else if(m_Header.dwSizeofImage >= 32 * 1024 * 1024)
        {
            fprintf(stderr, "Xbe::Xbe: Image size exceeds 32MiB\n");
        }

        // relocate to base : 0x00010000
        {
            printf("Xbe::Xbe: Relocating to Base 0x00010000...");

            uint32 fixCount = 0;

            uint32 relo_addr = x_Exe->m_OptionalHeader.m_image_data_directory[5].m_virtual_addr;
            uint32 relo_size = x_Exe->m_OptionalHeader.m_image_data_directory[5].m_size;

            uint32 dwBaseDiff = m_Header.dwPeBaseAddr - x_Exe->m_OptionalHeader.m_image_base;

            uint08 *reloc = GetAddr(relo_addr + m_Header.dwPeBaseAddr);

            // relocate, if necessary
            if(reloc != 0)
            {
                uint32 v = 0;

                // relocate each relocation block
                while(v < relo_size)
                {
                    uint32 block_addr = *(uint32 *)&reloc[v + 0];
                    uint32 block_stop = *(uint32 *)&reloc[v + 4] + v;

                    v += 8;

                    // relocate each rva
                    while(v < block_stop && v < relo_size)
                    {
                        uint16 data = *(uint16 *)&reloc[v];

                        uint32 type = (data & 0xF000) >> 12;

                        if(type == 0)
                        {
                            v += 2;
                            break;
                        }

                        // 32-bit field relocation
                        if(type == IMAGE_REL_BASED_HIGHLOW)
                        {
                            fixCount++;

                            uint32 dwFixAddr = block_addr + (data & 0x0FFF) + m_Header.dwPeBaseAddr;

                            uint08 *bzModRVA = GetAddr(dwFixAddr);

                            if(bzModRVA != 0)
                                *(uint32 *)bzModRVA += dwBaseDiff;
                        }
                        else
                        {
                            SetError("Unsupported relocation type", true);
                            goto cleanup;
                        }

                        v += 2;
                    }
                }
            }

            printf("OK (%d Fixups)\n", fixCount);
        }
    }

cleanup:

    if(GetError() != 0)
    {
        printf("FAILED!\n");
        printf("Xbe::Xbe: ERROR -> %s\n", GetError());
    }
}

// deconstructor
Xbe::~Xbe()
{
    if(m_bzSection != 0)
    {
        for(uint32 v = 0; v < m_Header.dwSections; v++)
            delete[] m_bzSection[v];

        delete[] m_bzSection;
    }

    delete m_XAPILibraryVersion;
    delete m_KernelLibraryVersion;
    delete[] m_LibraryVersion;
    delete m_TLS;
    delete[] m_szSectionName;
    delete[] m_SectionHeader;
    delete[] m_HeaderEx;
}

// export to Xbe file
void Xbe::Export(const char *x_szXbeFilename)
{
    if(GetError() != 0)
        return;

    char szBuffer[260];

    printf("Xbe::Export: Writing Xbe file...");

    FILE *XbeFile = fopen(x_szXbeFilename, "wb");

    // verify Xbe file was opened successfully
    if(XbeFile == NULL)
    {
        SetError("Could not open Xbe file", true);
        goto cleanup;
    }

    printf("OK\n");

    // write Xbe image header
    {
        printf("Xbe::Export: Writing Image Header...");

        if(fwrite(&m_Header, sizeof(m_Header), 1, XbeFile) != 1)
        {
            SetError("Unexpected write error while writing Xbe Image Header", false);
            goto cleanup;
        }

        printf("OK\n");

        printf("Xbe::Export: Writing Image Header Extra Bytes...");

        if(fwrite(m_HeaderEx, m_Header.dwSizeofHeaders, 1, XbeFile) != 1)
        {
            SetError("Unexpected write error while writing Xbe Image Header (Ex)", false);
            goto cleanup;
        }

        printf("OK\n");
    }

    // write Xbe certificate
    {
        printf("Xbe::Export: Writing Certificate...");

        fseek(XbeFile, m_Header.dwCertificateAddr - m_Header.dwBaseAddr, SEEK_SET);

        if(fwrite(&m_Certificate, sizeof(m_Certificate), 1, XbeFile) != 1)
        {
            SetError("Unexpected write error while writing Xbe Certificate", false);
            goto cleanup;
        }

        printf("OK\n");
    }

    // write Xbe section headers
    {
        printf("Xbe::Export: Writing Section Headers...\n");

        fseek(XbeFile, m_Header.dwSectionHeadersAddr - m_Header.dwBaseAddr, SEEK_SET);

        for(uint32 v = 0; v < m_Header.dwSections; v++)
        {
            printf("Xbe::Export: Writing Section Header 0x%.04X...", v);

            if(fwrite(&m_SectionHeader[v], sizeof(*m_SectionHeader), 1, XbeFile) != 1)
            {
                snprintf(szBuffer, sizeof(szBuffer),
                         "Unexpected write error while writing Xbe Section %d (%Xh)", v, v);
                SetError(szBuffer, false);
                goto cleanup;
            }

            printf("OK\n");
        }
    }

    // write Xbe sections
    {
        printf("Xbe::Export: Writing Sections...\n");

        for(uint32 v = 0; v < m_Header.dwSections; v++)
        {
            printf("Xbe::Export: Writing Section 0x%.04X (%s)...", v, m_szSectionName[v]);

            uint32 RawSize = m_SectionHeader[v].dwSizeofRaw;
            uint32 RawAddr = m_SectionHeader[v].dwRawAddr;

            fseek(XbeFile, RawAddr, SEEK_SET);

            if(RawSize == 0)
            {
                printf("OK\n");
                continue;
            }

            if(fwrite(m_bzSection[v], RawSize, 1, XbeFile) != 1)
            {
                snprintf(szBuffer, sizeof(szBuffer),
                         "Unexpected write error while writing Xbe Section %d (%Xh) (%s)", v, v,
                         m_szSectionName[v]);
                SetError(szBuffer, false);
                goto cleanup;
            }

            printf("OK\n");
        }
    }

    // zero pad
    {
        printf("Xbe::Export: Writing Zero Padding...");

        fpos_t pos;

        uint32 remaining = 0;

        fgetpos(XbeFile, &pos);

        remaining = (uint32)(0x1000 - ftell(XbeFile) % 0x1000);

        // write remaining bytes
        {
            char *szBuffer = new char[remaining];

            for(uint32 v = 0; v < remaining; v++)
                szBuffer[v] = 0;

            fwrite(szBuffer, remaining, 1, XbeFile);

            delete[] szBuffer;
        }

        printf("OK\n");
    }

cleanup:

    // if we came across an error, delete the file we were creating
    if(GetError() != 0)
    {
        remove(x_szXbeFilename);
        printf("FAILED!\n");
        printf("Xbe::Export: ERROR -> %s\n", GetError());
    }

    if(XbeFile != NULL)
    {
        fclose(XbeFile);
        XbeFile = NULL;
    }

    return;
}

// constructor initialization
void Xbe::ConstructorInit()
{
    m_HeaderEx = 0;
    m_SectionHeader = 0;
    m_szSectionName = 0;
    m_LibraryVersion = 0;
    m_KernelLibraryVersion = 0;
    m_XAPILibraryVersion = 0;
    m_TLS = 0;
    m_bzSection = 0;
}

// returns xbe timestamp date as string (reuses ctime buffer)
static char *XbeTime(uint32 timestamp)
{
    int v = 0;

    time_t time = timestamp;
    char *x_ctime = ctime(&time);

    for(v = 0; x_ctime[v] != '\n'; v++)
        ;

    x_ctime[v] = '\0';

    return x_ctime;
}

// dump Xbe information to text file
void Xbe::DumpInformation(FILE *x_file)
{
    if(GetError() != 0)
        return;

    time_t dwTimeDate = m_Header.dwTimeDate;

    fprintf(x_file, "XBE information generated by CXBE (Version: " VERSION ")\n");
    fprintf(x_file, "\n");
    fprintf(x_file, "Title identified as \"%s\"\n", m_szAsciiTitle);
    fprintf(x_file, "\n");
    fprintf(x_file, "Dumping XBE file header...\n");
    fprintf(x_file, "\n");
    fprintf(x_file, "Magic Number                     : XBEH\n");

    // print digital signature
    {
        fprintf(x_file, "Digitial Signature               : <Hex Dump>");
        for(int y = 0; y < 16; y++)
        {
            fprintf(x_file, "\n                                   ");
            for(int x = 0; x < 16; x++)
                fprintf(x_file, "%.02X", m_Header.pbDigitalSignature[y * 16 + x]);
        }
        fprintf(x_file, "\n                                   </Hex Dump>\n");
    }

    fprintf(x_file, "Base Address                     : 0x%.08X\n", m_Header.dwBaseAddr);
    fprintf(x_file, "Size of Headers                  : 0x%.08X\n", m_Header.dwSizeofHeaders);
    fprintf(x_file, "Size of Image                    : 0x%.08X\n", m_Header.dwSizeofImage);
    fprintf(x_file, "Size of Image Header             : 0x%.08X\n", m_Header.dwSizeofImageHeader);
    fprintf(x_file, "TimeDate Stamp                   : 0x%.08X (%s)\n", m_Header.dwTimeDate,
            XbeTime(dwTimeDate));
    fprintf(x_file, "Certificate Address              : 0x%.08X\n", m_Header.dwCertificateAddr);
    fprintf(x_file, "Number of Sections               : 0x%.08X\n", m_Header.dwSections);
    fprintf(x_file, "Section Headers Address          : 0x%.08X\n", m_Header.dwSectionHeadersAddr);

    // print init flags
    {
        fprintf(x_file, "Init Flags                       : 0x%.08X ",
                *(uint32 *)&m_Header.dwInitFlags);

        if(m_Header.dwInitFlags.bMountUtilityDrive)
            fprintf(x_file, "[Mount Utility Drive] ");

        if(m_Header.dwInitFlags.bFormatUtilityDrive)
            fprintf(x_file, "[Format Utility Drive] ");

        if(m_Header.dwInitFlags.bLimit64MB)
            fprintf(x_file, "[Limit Devkit Run Time Memory to 64MB] ");

        if(!m_Header.dwInitFlags.bDontSetupHarddisk)
            fprintf(x_file, "[Setup Harddisk] ");

        fprintf(x_file, "\n");
    }

    char AsciiFilename[40] = { 0 };

    setlocale(LC_ALL, "English");

    char *wszFilename = (char *)GetAddr(m_Header.dwDebugUnicodeFilenameAddr);

    if(wszFilename != NULL)
    {
        // wcstombs(AsciiFilename, wszFilename, 40);
        char *c = wszFilename;
        char *d = AsciiFilename;
        while(*c)
        {
            *d++ = *c++;
            c++;
        }
    }
    else
    {
        AsciiFilename[0] = '\0';
    }

    fprintf(x_file,
            "Entry Point                      : 0x%.08X (Retail: 0x%.08X, Debug: 0x%.08X)\n",
            m_Header.dwEntryAddr, m_Header.dwEntryAddr ^ XOR_EP_RETAIL,
            m_Header.dwEntryAddr ^ XOR_EP_DEBUG);
    fprintf(x_file, "TLS Address                      : 0x%.08X\n", m_Header.dwTLSAddr);
    fprintf(x_file, "(PE) Stack Commit                : 0x%.08X\n", m_Header.dwPeStackCommit);
    fprintf(x_file, "(PE) Heap Reserve                : 0x%.08X\n", m_Header.dwPeHeapReserve);
    fprintf(x_file, "(PE) Heap Commit                 : 0x%.08X\n", m_Header.dwPeHeapCommit);
    fprintf(x_file, "(PE) Base Address                : 0x%.08X\n", m_Header.dwPeBaseAddr);
    fprintf(x_file, "(PE) Size of Image               : 0x%.08X\n", m_Header.dwPeSizeofImage);
    fprintf(x_file, "(PE) Checksum                    : 0x%.08X\n", m_Header.dwPeChecksum);
    fprintf(x_file, "(PE) TimeDate Stamp              : 0x%.08X (%s)\n", m_Header.dwPeTimeDate,
            XbeTime(m_Header.dwPeTimeDate));
    fprintf(x_file, "Debug Pathname Address           : 0x%.08X (\"%s\")\n",
            m_Header.dwDebugPathnameAddr, GetAddr(m_Header.dwDebugPathnameAddr));
    fprintf(x_file, "Debug Filename Address           : 0x%.08X (\"%s\")\n",
            m_Header.dwDebugFilenameAddr, GetAddr(m_Header.dwDebugFilenameAddr));
    fprintf(x_file, "Debug Unicode filename Address   : 0x%.08X (L\"%s\")\n",
            m_Header.dwDebugUnicodeFilenameAddr, AsciiFilename);
    fprintf(x_file,
            "Kernel Image Thunk Address       : 0x%.08X (Retail: 0x%.08X, Debug: 0x%.08X)\n",
            m_Header.dwKernelImageThunkAddr, m_Header.dwKernelImageThunkAddr ^ XOR_KT_RETAIL,
            m_Header.dwKernelImageThunkAddr ^ XOR_KT_DEBUG);
    fprintf(
        x_file, "NonKernel Import Dir Address     : 0x%.08X\n", m_Header.dwNonKernelImportDirAddr);
    fprintf(x_file, "Library Versions                 : 0x%.08X\n", m_Header.dwLibraryVersions);
    fprintf(x_file, "Library Versions Address         : 0x%.08X\n", m_Header.dwLibraryVersionsAddr);
    fprintf(x_file, "Kernel Library Version Address   : 0x%.08X\n",
            m_Header.dwKernelLibraryVersionAddr);
    fprintf(
        x_file, "XAPI Library Version Address     : 0x%.08X\n", m_Header.dwXAPILibraryVersionAddr);
    fprintf(x_file, "Logo Bitmap Address              : 0x%.08X\n", m_Header.dwLogoBitmapAddr);
    fprintf(x_file, "Logo Bitmap Size                 : 0x%.08X\n", m_Header.dwSizeofLogoBitmap);
    fprintf(x_file, "\n");
    fprintf(x_file, "Dumping XBE Certificate...\n");
    fprintf(x_file, "\n");
    fprintf(x_file, "Size of Certificate              : 0x%.08X\n", m_Certificate.dwSize);
    fprintf(x_file, "TimeDate Stamp                   : 0x%.08X (%s)\n", m_Certificate.dwTimeDate,
            XbeTime(m_Certificate.dwTimeDate));
    fprintf(x_file, "Title ID                         : 0x%.08X\n", m_Certificate.dwTitleId);
    fprintf(x_file, "Title                            : L\"%s\"\n", m_szAsciiTitle);

    // print alternate title IDs
    {
        fprintf(x_file, "Alternate Titles IDs             : ");

        for(int v = 0; v < 0x10; v++)
        {
            if(v != 0)
                fprintf(x_file, "                                   ");
            fprintf(x_file, "0x%.08X", m_Certificate.dwAlternateTitleId[v]);
            if(v != 0x0F)
                fprintf(x_file, "\n");
        }

        fprintf(x_file, "\n");
    }

    fprintf(x_file, "Allowed Media                    : 0x%.08X\n", m_Certificate.dwAllowedMedia);
    fprintf(x_file, "Game Region                      : 0x%.08X\n", m_Certificate.dwGameRegion);
    fprintf(x_file, "Game Ratings                     : 0x%.08X\n", m_Certificate.dwGameRatings);
    fprintf(x_file, "Disk Number                      : 0x%.08X\n", m_Certificate.dwDiskNumber);
    fprintf(x_file, "Version                          : 0x%.08X\n", m_Certificate.dwVersion);

    // print LAN key
    {
        fprintf(x_file, "LAN Key                          : ");
        for(int x = 0; x < 16; x++)
            fprintf(x_file, "%.02X", m_Certificate.bzLanKey[x]);
        fprintf(x_file, "\n");
    }

    // print signature key
    {
        fprintf(x_file, "Signature Key                    : ");
        for(int x = 0; x < 16; x++)
            fprintf(x_file, "%.02X", m_Certificate.bzSignatureKey[x]);
        fprintf(x_file, "\n");
    }

    // print alternate signature keys
    {
        fprintf(x_file, "Title Alternate Signature Keys : <Hex Dump>");
        for(int y = 0; y < 16; y++)
        {
            fprintf(x_file, "\n                                   ");
            for(int x = 0; x < 16; x++)
                fprintf(x_file, "%.02X", m_Certificate.bzTitleAlternateSignatureKey[y][x]);
        }
        fprintf(x_file, "\n                                   </Hex Dump>\n");
    }

    fprintf(x_file, "\n");
    fprintf(x_file, "Dumping XBE Section Headers...\n");
    fprintf(x_file, "\n");

    // print section headers
    {
        for(uint32 v = 0; v < m_Header.dwSections; v++)
        {
            fprintf(x_file, "Section Name                     : 0x%.08X (\"%s\")\n",
                    m_SectionHeader[v].dwSectionNameAddr, m_szSectionName[v]);

            // print flags
            {
                fprintf(x_file, "Flags                            : 0x%.08X ",
                        *(uint32 *)&m_SectionHeader[v].dwFlags);

                if(m_SectionHeader[v].dwFlags.bWritable)
                    fprintf(x_file, "(Writable) ");

                if(m_SectionHeader[v].dwFlags.bPreload)
                    fprintf(x_file, "(Preload) ");

                if(m_SectionHeader[v].dwFlags.bExecutable)
                    fprintf(x_file, "(Executable) ");

                if(m_SectionHeader[v].dwFlags.bInsertedFile)
                    fprintf(x_file, "(Inserted File) ");

                if(m_SectionHeader[v].dwFlags.bHeadPageRO)
                    fprintf(x_file, "(Head Page RO) ");

                if(m_SectionHeader[v].dwFlags.bTailPageRO)
                    fprintf(x_file, "(Tail Page RO) ");

                fprintf(x_file, "\n");
            }

            fprintf(x_file, "Virtual Address                  : 0x%.08X\n",
                    m_SectionHeader[v].dwVirtualAddr);
            fprintf(x_file, "Virtual Size                     : 0x%.08X\n",
                    m_SectionHeader[v].dwVirtualSize);
            fprintf(x_file, "Raw Address                      : 0x%.08X\n",
                    m_SectionHeader[v].dwRawAddr);
            fprintf(x_file, "Size of Raw                      : 0x%.08X\n",
                    m_SectionHeader[v].dwSizeofRaw);
            fprintf(x_file, "Section Name Address             : 0x%.08X\n",
                    m_SectionHeader[v].dwSectionNameAddr);
            fprintf(x_file, "Section Reference Count          : 0x%.08X\n",
                    m_SectionHeader[v].dwSectionRefCount);
            fprintf(x_file, "Head Shared Reference Count Addr : 0x%.08X\n",
                    m_SectionHeader[v].dwHeadSharedRefCountAddr);
            fprintf(x_file, "Tail Shared Reference Count Addr : 0x%.08X\n",
                    m_SectionHeader[v].dwTailSharedRefCountAddr);

            // print section digest
            {
                fprintf(x_file, "Section Digest                   : ");
                for(int s = 0; s < 20; s++)
                    fprintf(x_file, "%.02X", m_SectionHeader[v].bzSectionDigest[s]);
                fprintf(x_file, "\n");
            }

            fprintf(x_file, "\n");
        }
    }

    fprintf(x_file, "Dumping XBE Library Versions...\n");
    fprintf(x_file, "\n");

    // print library versions
    {
        if(m_LibraryVersion == 0 || m_Header.dwLibraryVersions == 0)
        {
            fprintf(x_file, "(This XBE contains no Library Versions)\n");
            fprintf(x_file, "\n");
        }
        else
        {
            for(uint32 v = 0; v < m_Header.dwLibraryVersions; v++)
            {
                char tmp[9];

                for(uint32 c = 0; c < 8; c++)
                    tmp[c] = m_LibraryVersion[v].szName[c];

                tmp[8] = '\0';

                fprintf(x_file, "Library Name                     : %s\n", tmp);
                fprintf(x_file, "Version                          : %d.%d.%d\n",
                        m_LibraryVersion[v].wMajorVersion, m_LibraryVersion[v].wMinorVersion,
                        m_LibraryVersion[v].wBuildVersion);

                // print flags
                {
                    fprintf(x_file, "Flags                            : ");

                    fprintf(
                        x_file, "QFEVersion : 0x%.04X, ", m_LibraryVersion[v].dwFlags.QFEVersion);

                    if(m_LibraryVersion[v].dwFlags.bDebugBuild)
                        fprintf(x_file, "Debug, ");
                    else
                        fprintf(x_file, "Retail, ");

                    switch(m_LibraryVersion[v].dwFlags.Approved)
                    {
                        case 0:
                            fprintf(x_file, "Unapproved");
                            break;
                        case 1:
                            fprintf(x_file, "Possibly Approved");
                            break;
                        case 2:
                            fprintf(x_file, "Approved");
                            break;
                    }

                    fprintf(x_file, "\n");
                }

                fprintf(x_file, "\n");
            }
        }
    }

    fprintf(x_file, "Dumping XBE TLS...\n");
    fprintf(x_file, "\n");

    // print thread local storage
    if(m_TLS != 0)
    {
        fprintf(x_file, "Data Start Address               : 0x%.08X\n", m_TLS->dwDataStartAddr);
        fprintf(x_file, "Data End Address                 : 0x%.08X\n", m_TLS->dwDataEndAddr);
        fprintf(x_file, "TLS Index Address                : 0x%.08X\n", m_TLS->dwTLSIndexAddr);
        fprintf(x_file, "TLS Callback Address             : 0x%.08X\n", m_TLS->dwTLSCallbackAddr);
        fprintf(x_file, "Size of Zero Fill                : 0x%.08X\n", m_TLS->dwSizeofZeroFill);
        fprintf(x_file, "Characteristics                  : 0x%.08X\n", m_TLS->dwCharacteristics);
    }
    else
    {
        fprintf(x_file, "(This XBE contains no TLS)\n");
    }
}

// import logo bitmap from raw monochrome data
void Xbe::ImportLogoBitmap(const uint08 x_Gray[100 * 17])
{
    char *LogoBuffer = new char[4 * 1024];
    uint32 LogoSize = 0;

    // encode logo bitmap
    {
        for(uint32 v = 1; v < 100 * 17; LogoSize++)
        {
            char color = x_Gray[v] >> 4;

            uint32 len = 1;

            while(++v < 100 * 17 - 1 && len < 1024 && color == x_Gray[v] >> 4)
                len++;

            LogoRLE *cur = (LogoRLE *)&LogoBuffer[LogoSize];

            if(len <= 7)
            {
                cur->m_Eight.bType1 = 1;
                cur->m_Eight.Len = len;
                cur->m_Eight.Data = color;
            }
            else
            {
                cur->m_Sixteen.bType1 = 0;
                cur->m_Sixteen.bType2 = 1;
                cur->m_Sixteen.Len = len;
                cur->m_Sixteen.Data = color;
                LogoSize++;
            }
        }
    }

    // check if there is room to save this, if not then throw an error
    {
        uint08 *RLE = GetLogoBitmap(LogoSize);

        if(RLE == 0)
        {
            if(GetError() == 0)
                SetError("Logo bitmap could not be imported (not enough space in file?)", false);

            return;
        }

        memcpy(RLE, LogoBuffer, LogoSize);
    }

    return;
}

// ******************************************************************
// * ExportLogoBitmap
// ******************************************************************
// *
// * This algorithm was originally discovered by superfro. I couldnt
// * figure out what the hell the encoding format was before he gave
// * me the information:
// *
// * basically what is going on here is a single pass through the
// * bitmap data, with 2 possible encodings per rle chunk. data is
// * stored as 4 bit grayscale, so the logical approach is to expand
// * this to 8 bit using a simple 4 bit left shift (*16). However, it
// * has been suggested to me by superfro that you might calculate a
// * slightly darker image by multiplying by 15 and adding .5. It's
// * a toss up, but i've choosen a simple bit shift left.
// *
// ******************************************************************
void Xbe::ExportLogoBitmap(uint08 x_Gray[100 * 17])
{
    memset(x_Gray, 0, 100 * 17);

    uint32 dwLength = m_Header.dwSizeofLogoBitmap;

    uint08 *RLE = GetAddr(m_Header.dwLogoBitmapAddr);

    if(RLE == 0 || GetError())
        return;

    uint32 o = 0;

    for(uint32 v = 0; v < dwLength; v++)
    {
        uint32 len = 0, data = 0;

        LogoRLE *cur = (LogoRLE *)&RLE[v];

        if(cur->m_Eight.bType1)
        {
            len = cur->m_Eight.Len;
            data = cur->m_Eight.Data;
        }
        else
        {
            if(cur->m_Sixteen.bType2)
            {
                len = cur->m_Sixteen.Len;
                data = cur->m_Sixteen.Data;
                v += 1;
            }
        }

        for(uint32 j = 0; j < len; j++)
        {
            o++;

            if(o < 100 * 17)
                x_Gray[o] = (char)(data << 4); // could use (int)(data * 15.0 + .5);
        }
    }
}

// return a modifiable pointer inside this structure that corresponds to a virtual address
uint08 *Xbe::GetAddr(uint32 x_dwVirtualAddress)
{
    uint32 dwOffs = x_dwVirtualAddress - m_Header.dwBaseAddr;

    // offset into image header
    if(dwOffs < sizeof(m_Header))
        return &((uint08 *)&m_Header)[dwOffs];

    // offset into image header extra bytes
    if(dwOffs < m_Header.dwSizeofHeaders)
        return (uint08 *)&m_HeaderEx[dwOffs - sizeof(m_Header)];

    // offset into some random section
    {
        for(uint32 v = 0; v < m_Header.dwSections; v++)
        {
            uint32 VirtAddr = m_SectionHeader[v].dwVirtualAddr;
            uint32 VirtSize = m_SectionHeader[v].dwVirtualSize;

            if((x_dwVirtualAddress >= VirtAddr) && (x_dwVirtualAddress < (VirtAddr + VirtSize)))
                return &m_bzSection[v][x_dwVirtualAddress - VirtAddr];
        }
    }

    return 0;
}

// return a modifiable pointer to logo bitmap data
uint08 *Xbe::GetLogoBitmap(uint32 x_dwSize)
{
    uint32 dwOffs = m_Header.dwLogoBitmapAddr - m_Header.dwBaseAddr;
    uint32 dwLength = m_Header.dwSizeofLogoBitmap;

    if(dwOffs == 0 || dwLength == 0)
        return 0;

    // if this bitmap will fit inside the already existing one, we don't need to resize, just return
    // pointer
    if(dwLength >= x_dwSize)
    {
        // update size of headers, if necessary
        if(dwOffs < m_Header.dwSizeofHeaders)
        {
            m_Header.dwSizeofHeaders -= dwLength;
            m_Header.dwSizeofHeaders += x_dwSize;
            m_Header.dwSizeofLogoBitmap = x_dwSize;

            return GetAddr(m_Header.dwLogoBitmapAddr);
        }
    }

    return 0;
}

// returns the count of import table entries other than the kernel.
// extra_bytes will be populated with the number of bytes necessary to hold the
// import names after conversion to wide character strings.
static uint32 CountNonKernelImportTableEntries(class Exe *x_Exe, uint32_t *extra_bytes)
{
    *extra_bytes = 0;
    uint32 idtRVA =
        x_Exe->m_OptionalHeader.m_image_data_directory[IMAGE_DIRECTORY_ENTRY_IMPORT].m_virtual_addr;
    if(idtRVA == 0)
    {
        return 0;
    }

    uint32 count = 0;
    const IMAGE_IMPORT_DESCRIPTOR *descr =
        reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR *>(x_Exe->ReadAddr(idtRVA));
    for(; descr->FirstThunk; ++descr)
    {
        uint32 nameRVA = descr->Name;
        const char *name = reinterpret_cast<const char *>(x_Exe->ReadAddr(nameRVA));
        if(strcasecmp(name, kKernelImageName) != 0)
        {
            *extra_bytes += (strlen(name) + 1) * 2;
            ++count;
        }
    }

    return count;
}

// sets the dwKernelImageThunkAddr and dwNonKernelImportDirAddr fields from the given Exe.
bool Xbe::ProcessImportTable(class Exe *x_Exe, bool x_bRetail)
{
    uint32 idtRVA =
        x_Exe->m_OptionalHeader.m_image_data_directory[IMAGE_DIRECTORY_ENTRY_IMPORT].m_virtual_addr;
    if(idtRVA == 0)
    {
        printf("Xbe::ProcessImportTable: No import directory found, exe may be invalid.\n");
        return true;
    }

    uint32 kernel_thunk_addr = 0;
    const IMAGE_IMPORT_DESCRIPTOR *peDescriptor =
        reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR *>(GetAddr(idtRVA + m_Header.dwPeBaseAddr));
    XBE_IMAGE_IMPORT_DESCRIPTOR *xbeDescriptor = nullptr;
    char *wideCharName = nullptr;
    uint32 nextImportNameAddr = m_ImportNameAddr;
    if(m_Header.dwNonKernelImportDirAddr)
    {
        xbeDescriptor = reinterpret_cast<XBE_IMAGE_IMPORT_DESCRIPTOR *>(
            GetAddr(m_Header.dwNonKernelImportDirAddr));
        if(!xbeDescriptor)
        {
            SetError("Xbe::ProcessImportTable: Non kernel imports exist but target descriptor "
                     "table is null",
                     true);
            return false;
        }

        wideCharName = reinterpret_cast<char *>(GetAddr(m_ImportNameAddr));
        if(!wideCharName)
        {
            SetError(
                "Xbe::ProcessImportTable: Non kernel imports exist but target name table is null",
                true);
            return false;
        }
    }

    for(int32 i = 0; peDescriptor->FirstThunk; ++i, ++peDescriptor)
    {
        uint32 nameRVA = peDescriptor->Name;
        const char *name = reinterpret_cast<const char *>(GetAddr(nameRVA + m_Header.dwPeBaseAddr));
        if(!strcasecmp(name, kKernelImageName))
        {
            kernel_thunk_addr = peDescriptor->FirstThunk + m_Header.dwPeBaseAddr;
        }
        else if(xbeDescriptor)
        {
            xbeDescriptor->FirstThunk = peDescriptor->FirstThunk + m_Header.dwPeBaseAddr;
            xbeDescriptor->WideCharName = nextImportNameAddr;

            size_t len = strlen(name) + 1;
            for(uint32 ch = 0; ch < len; ++ch)
            {
                *wideCharName++ = name[ch];
                *wideCharName++ = 0;
                nextImportNameAddr += 2;
            }
            ++xbeDescriptor;
        }
    }

    if(kernel_thunk_addr == 0)
    {
        printf("Xbe::ProcessImportTable: No kernel (%s) imports, exe may be invalid.\n",
               kKernelImageName);
    }

    kernel_thunk_addr ^= (x_bRetail ? XOR_KT_RETAIL : XOR_KT_DEBUG);

    m_Header.dwKernelImageThunkAddr = kernel_thunk_addr;

    if(xbeDescriptor)
    {
        if(x_bRetail)
        {
            printf("Xbe::ProcessImportTable: Non-kernel imports found in retail image, exe may be "
                   "invalid.\n");
        }
        memset(xbeDescriptor, 0, sizeof(*xbeDescriptor));
    }

    return true;
}
