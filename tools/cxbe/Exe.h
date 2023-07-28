// This file is part of Cxbe
// SPDX-License-Identifier: GPL-2.0-or-later

// SPDX-FileCopyrightText: 2002-2003 Aaron Robinson <caustik@caustik.com>
// SPDX-FileCopyrightText: 2021 Stefan Schmidt

#ifndef EXE_H
#define EXE_H

#include <string>

#include "Error.h"

typedef struct IMAGE_IMPORT_DESCRIPTOR
{
    union {
        uint32 Characteristics;
        uint32 OriginalFirstThunk;
    } DUMMYUNIONNAME;
    uint32 TimeDateStamp;
    uint32 ForwarderChain;
    uint32 Name;
    uint32 FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR;

// Exe (PE) file object
class Exe : public Error
{
  public:
    // construct via Exe file
    Exe(const char *x_szFilename);

    // deconstructor
    ~Exe();

    // export to Exe file
    void Export(const char *x_szExeFilename);

    // DOSHeader
    struct DOSHeader
    {
        uint16 m_magic;    // DOS .EXE magic number
        uint16 m_cblp;     // byte on last page
        uint16 m_cp;       // number of pages
        uint16 m_crlc;     // number of relocations
        uint16 m_cparhdr;  // size of header (in paragraphs)
        uint16 m_minalloc; // minimum extra paragraphs needed
        uint16 m_maxalloc; // maximum extra paragraphs needed
        uint16 m_ss;       // initial SS value (relative)
        uint16 m_sp;       // initial SP value
        uint16 m_csum;     // checksum
        uint16 m_ip;       // initial IP value
        uint16 m_cs;       // initial CS value (relative)
        uint16 m_lfarlc;   // file address of relocation table
        uint16 m_ovno;     // overlay number
        uint16 m_res[4];   // reserved words
        uint16 m_oemid;    // OEM identifier
        uint16 m_oeminfo;  // OEM information
        uint16 m_res2[10]; // reserved words
        uint32 m_lfanew;   // file address of new .EXE header
    } __attribute((packed)) m_DOSHeader;

    // Header (PE)
    struct Header
    {
        uint32 m_magic;                  // magic number [should be "PE\0\0"]
        uint16 m_machine;                // machine type
        uint16 m_sections;               // number of sections
        uint32 m_timedate;               // timedate stamp
        uint32 m_symbol_table_addr;      // symbol table address
        uint32 m_symbols;                // number of symbols
        uint16 m_sizeof_optional_header; // size of optional header
        uint16 m_characteristics;        // characteristics
    } __attribute((packed)) m_Header;

    // optional header (PE)
    struct OptionalHeader
    {
        uint16 m_magic;                     // magic number [should be 0x010B]
        uint08 m_linker_version_major;      // linker version [major]
        uint08 m_linker_version_minor;      // linker version [minor]
        uint32 m_sizeof_code;               // size of code
        uint32 m_sizeof_initialized_data;   // size of initialized data
        uint32 m_sizeof_uninitialized_data; // size of uninitialized data
        uint32 m_entry;                     // address of entry point
        uint32 m_code_base;                 // address of code base
        uint32 m_data_base;                 // address of data base
        uint32 m_image_base;                // address of image base
        uint32 m_section_alignment;         // section alignment
        uint32 m_file_alignment;            // file alignment
        uint16 m_os_version_major;          // operating system version [major]
        uint16 m_os_version_minor;          // operating system version [minor]
        uint16 m_image_version_major;       // image version [major]
        uint16 m_image_version_minor;       // image version [minor]
        uint16 m_subsystem_version_major;   // subsystem version [major]
        uint16 m_subsystem_version_minor;   // subsystem version [minor]
        uint32 m_win32_version;             // win32 version
        uint32 m_sizeof_image;              // size of image
        uint32 m_sizeof_headers;            // size of headers
        uint32 m_checksum;                  // checksum
        uint16 m_subsystem;                 // subsystem
        uint16 m_dll_characteristics;       // dll characteristics
        uint32 m_sizeof_stack_reserve;      // size of stack reserve
        uint32 m_sizeof_stack_commit;       // size of stack commit
        uint32 m_sizeof_heap_reserve;       // size of heap reserve
        uint32 m_sizeof_heap_commit;        // size of heap commit
        uint32 m_loader_flags;              // loader flags
        uint32 m_data_directories;          // data directories

        struct image_data_directory // image data directory
        {
            uint32 m_virtual_addr; // virtual address
            uint32 m_size;         // size
        } m_image_data_directory[0x10];
    } __attribute((packed)) m_OptionalHeader;

    // PE section header
    struct SectionHeader
    {
        uint08 m_name[8];          // name of section
        uint32 m_virtual_size;     // virtual size of segment
        uint32 m_virtual_addr;     // virtual address of segment
        uint32 m_sizeof_raw;       // size of raw data
        uint32 m_raw_addr;         // address of raw data
        uint32 m_relocations_addr; // address of relocations
        uint32 m_linenumbers_addr; // address of line numbers
        uint16 m_relocations;      // number of relocations
        uint16 m_linenumbers;      // number of linenumbers
        uint32 m_characteristics;  // characteristics for this segment
    } __attribute((packed)) * m_SectionHeader;

    // array to store long section names
    struct SectionHeader_longname
    {
        char *m_longname;
        uint32 m_offset;
    } __attribute((packed)) * m_SectionHeader_longname;

    // array of section data
    uint08 **m_bzSection;

    // return a const pointer inside this structure that corresponds to a virtual address
    const uint08 *ReadAddr(uint32 x_dwVirtualAddress) const;

  protected:
    // protected default constructor
    Exe(){};

    // constructor initialization
    void ConstructorInit();

    // return a modifiable pointer inside this structure that corresponds to a virtual address
    uint08 *GetAddr(uint32 x_dwVirtualAddress);
};

// PE file/segment alignments : these must always both equal 0x0020
#define PE_FILE_ALIGN 0x00000020
#define PE_SEGM_ALIGN 0x00000020

// section characteristics
#define IMAGE_SCN_CNT_CODE 0x00000020
#define IMAGE_SCN_CNT_INITIALIZED_DATA 0x00000040
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA 0x00000080
#define IMAGE_SCN_MEM_DISCARDABLE 0x02000000
#define IMAGE_SCN_MEM_NOT_CACHED 0x04000000
#define IMAGE_SCN_MEM_NOT_PAGED 0x08000000
#define IMAGE_SCN_MEM_SHARED 0x10000000
#define IMAGE_SCN_MEM_EXECUTE 0x20000000
#define IMAGE_SCN_MEM_READ 0x40000000
#define IMAGE_SCN_MEM_WRITE 0x80000000

// based relocation types
#define IMAGE_REL_BASED_ABSOLUTE 0
#define IMAGE_REL_BASED_HIGH 1
#define IMAGE_REL_BASED_LOW 2
#define IMAGE_REL_BASED_HIGHLOW 3
#define IMAGE_REL_BASED_HIGHADJ 4
#define IMAGE_REL_BASED_MIPS_JMPADDR 5
#define IMAGE_REL_BASED_MIPS_JMPADDR16 9
#define IMAGE_REL_BASED_IA64_IMM64 9
#define IMAGE_REL_BASED_DIR64 10

// machine type(s)
#define IMAGE_FILE_MACHINE_I386 0x014c // Intel 386

// subsystems
#define IMAGE_SUBSYSTEM_UNKNOWN 0
#define IMAGE_SUBSYSTEM_NATIVE 1
#define IMAGE_SUBSYSTEM_WINDOWS_GUI 2
#define IMAGE_SUBSYSTEM_WINDOWS_CUI 3
#define IMAGE_SUBSYSTEM_OS2_CUI 5
#define IMAGE_SUBSYSTEM_POSIX_CUI 7
#define IMAGE_SUBSYSTEM_NATIVE_WINDOWS 8
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI 9

// directory entries
#define IMAGE_DIRECTORY_ENTRY_EXPORT 0    // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT 1    // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE 2  // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION 3 // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY 4  // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC 5 // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG 6     // Debug Directory
//      IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE 7    // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR 8       // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS 9             // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG 10    // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT 11   // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT 12            // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT 13   // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14 // COM Runtime descriptor

// typical DOS stub
static uint08 bzDOSStub[] = {
    0x4D, 0x5A, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
    0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x00,
    0x0E, 0x1F, 0xBA, 0x0E, 0x00, 0xB4, 0x09, 0xCD, 0x21, 0xB8, 0x01, 0x4C, 0xCD, 0x21, 0x54, 0x68,
    0x69, 0x73, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72, 0x61, 0x6D, 0x20, 0x63, 0x61, 0x6E, 0x6E, 0x6F,
    0x74, 0x20, 0x62, 0x65, 0x20, 0x72, 0x75, 0x6E, 0x20, 0x69, 0x6E, 0x20, 0x44, 0x4F, 0x53, 0x20,
    0x6D, 0x6F, 0x64, 0x65, 0x2E, 0x0D, 0x0D, 0x0A, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x85, 0xE3, 0xB8, 0xDB, 0xC1, 0x82, 0xD6, 0x88, 0xC1, 0x82, 0xD6, 0x88, 0xC1, 0x82, 0xD6, 0x88,
    0xC1, 0x82, 0xD7, 0x88, 0xC3, 0x82, 0xD6, 0x88, 0x3E, 0xA2, 0xD2, 0x88, 0xC2, 0x82, 0xD6, 0x88,
    0x95, 0xA1, 0xE7, 0x88, 0xC0, 0x82, 0xD6, 0x88, 0x52, 0x69, 0x63, 0x68, 0xC1, 0x82, 0xD6, 0x88,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#endif
