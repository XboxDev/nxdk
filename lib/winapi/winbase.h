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

#define FILE_BEGIN 0
#define FILE_CURRENT 1
#define FILE_END 2
#define INVALID_SET_FILE_POINTER ((DWORD)-1)

#define FILE_FLAG_WRITE_THROUGH 0x80000000
#define FILE_FLAG_OVERLAPPED 0x40000000
#define FILE_FLAG_NO_BUFFERING 0x20000000
#define FILE_FLAG_RANDOM_ACCESS 0x10000000
#define FILE_FLAG_SEQUENTIAL_SCAN 0x08000000
#define FILE_FLAG_DELETE_ON_CLOSE 0x04000000
#define FILE_FLAG_BACKUP_SEMANTICS 0x02000000
#define FILE_FLAG_POSIX_SEMANTICS 0x01000000

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

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
