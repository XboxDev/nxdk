#include <libloaderapi.h>
#include <winbase.h>
#include <windef.h>
#include <winerror.h>
#include <xboxkrnl/xboxkrnl.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

HMODULE LoadLibraryExA (LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
    assert(hFile == NULL);
    assert(dwFlags == 0);

    // Always fail with not having found the library
    SetLastError(ERROR_MOD_NOT_FOUND);
    return NULL;
}

HMODULE LoadLibraryA (LPCSTR lpLibFileName)
{
    return LoadLibraryExA(lpLibFileName, NULL, 0);
}

BOOL FreeLibrary (HMODULE hLibModule)
{
    assert(hLibModule != NULL);

    // Always claim success when free'ing a library
    return TRUE;
}

static PIMAGE_EXPORT_DIRECTORY find_edata (void)
{
    DWORD num_sections = CURRENT_XBE_HEADER->NumberOfSections;
    PXBE_SECTION_HEADER section_header_addr = CURRENT_XBE_HEADER->PointerToSectionTable;

    for (DWORD i = 0; i < num_sections; i++) {
        if (strcmp(section_header_addr[i].SectionName, ".edata") == 0) {
            return (PIMAGE_EXPORT_DIRECTORY)section_header_addr[i].VirtualAddress;
        }
    }

    return NULL;
}

FARPROC GetProcAddress (HMODULE hModule, LPCSTR lpProcName)
{
    if (hModule == NULL) {
        // When no dll handle is given, the symbol gets looked up in the main module

        PIMAGE_EXPORT_DIRECTORY exportdir = find_edata();
        if (!exportdir) {
            SetLastError(ERROR_PROC_NOT_FOUND);
            return NULL;
        }

        for (DWORD i = 0; i < exportdir->NumberOfNames; i++) {
            const char **nametable = (const char **)(exportdir->AddressOfNames + XBE_DEFAULT_BASE);
            const char *name_addr = (const char *)(nametable[i] + XBE_DEFAULT_BASE);

            if (strcmp(lpProcName, name_addr) == 0) {
                // Found a matching name and its index. This index is not valid for the address table, that index needs to be looked up in the ordinal table!
                WORD *ordtable = (WORD *)(exportdir->AddressOfNameOrdinals + XBE_DEFAULT_BASE);
                BYTE **proctable = (BYTE **)(exportdir->AddressOfFunctions + XBE_DEFAULT_BASE);
                return (FARPROC)proctable[ordtable[i]] + XBE_DEFAULT_BASE;
            }
        }

        SetLastError(ERROR_PROC_NOT_FOUND);
        return NULL;
    }

    // FIXME: If the module handle is invalid, fail with ERROR_MOD_NOT_FOUND

    // Always fail with not having found the export
    SetLastError(ERROR_PROC_NOT_FOUND);
    return NULL;
}
