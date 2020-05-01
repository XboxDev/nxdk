#include <libloaderapi.h>
#include <winbase.h>
#include <windef.h>
#include <winerror.h>
#include <xboxkrnl/xboxkrnl.h>

#include <assert.h>
#include <stdlib.h>

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

FARPROC GetProcAddress (HMODULE hModule, LPCSTR lpProcName)
{
    // FIXME: If hModule is NULL, the symbol is looked up in the current module

    // FIXME: If the module handle is invalid, fail with ERROR_MOD_NOT_FOUND

    // Always fail with not having found the export
    SetLastError(ERROR_PROC_NOT_FOUND);
    return NULL;
}
