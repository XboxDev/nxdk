#ifndef __WINBASE_H__
#define __WINBASE_H__

#include <windef.h>
#include <minwinbase.h>
#include <winnt.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define INVALID_HANDLE_VALUE ((HANDLE)-1)

typedef struct _WIN32_FIND_DATAA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR cFileName[MAX_PATH];
    CHAR cAlternateFileName[14];
} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;

#ifndef UNICODE
#define WIN32_FIND_DATA WIN32_FIND_DATAA
#define PWIN32_FIND_DATA PWIN32_FIND_DATAA
#define LPWIN32_FIND_DATA LPWIN32_FIND_DATAA
#endif

DWORD GetLastError (void);
void SetLastError (DWORD error);

void WINAPI OutputDebugStringA (LPCTSTR lpOutputString);

#ifndef UNICODE
#define OutputDebugString OutputDebugStringA
#else
#error nxdk does not support the Unicode API
#endif

#ifdef __cplusplus
}
#endif

#endif
