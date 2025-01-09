// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2020 Stefan Schmidt
// SPDX-FileCopyrightText: 2019-2020 Jannik Vogel

#ifndef __WINBASE_H__
#define __WINBASE_H__

#include <basetsd.h>
#include <minwinbase.h>
#include <windef.h>
#include <winnt.h>

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_HANDLE_VALUE ((HANDLE) - 1)

#define INFINITE 0xFFFFFFFF

#define CREATE_SUSPENDED 0x00000004
// The STACK_SIZE_PARAM_IS_A_RESERVATION flag has no effect when passed to
// CreateThread or _beginthreadex, stacks are always committed memory in the Xbox
#define STACK_SIZE_PARAM_IS_A_RESERVATION 0x00010000

#define THREAD_PRIORITY_ABOVE_NORMAL  1
#define THREAD_PRIOIRTY_BELOW_NORMAL  (-1)
#define THREAD_PRIORITY_HIGHEST       2
#define THREAD_PRIORITY_IDLE          (-15)
#define THREAD_PRIORITY_LOWEST        (-2)
#define THREAD_PRIORITY_NORMAL        0
#define THREAD_PRIORITY_TIME_CRITICAL 15
#define THREAD_PRIORITY_ERROR_RETURN  0x7FFFFFFF

typedef VOID(WINAPI *PFLS_CALLBACK_FUNCTION)(PVOID);
#define FLS_OUT_OF_INDEXES    0xFFFFFFFF
#define FLS_MAXIMUM_AVAILABLE 64
#define TLS_OUT_OF_INDEXES    FLS_OUT_OF_INDEXES
#define TLS_MINIMUM_AVAILABLE FLS_MAXIMUM_AVAILABLE

typedef struct _INIT_ONCE
{
    PVOID Ptr;
} INIT_ONCE, *PINIT_ONCE, *LPINIT_ONCE;
#define INIT_ONCE_STATIC_INIT       {0}
#define INIT_ONCE_CTX_RESERVED_BITS 2
#define INIT_ONCE_CHECK_ONLY        0x00000001UL
#define INIT_ONCE_ASYNC             0x00000002UL
#define INIT_ONCE_INIT_FAILED       0x00000004UL
typedef BOOL(CALLBACK *PINIT_ONCE_FN)(PINIT_ONCE, PVOID, PVOID *);

typedef struct _CONDITION_VARIABLE
{
    INIT_ONCE initOnce;
    // eventHandles[0]: Created as SynchronizationEvent to wake a single thread
    // eventHandles[1]: Created as NotificationEvent to wake all threads at once
    PVOID eventHandles[2];
    int waitCount;
} CONDITION_VARIABLE, *PCONDITION_VARIABLE;
#define CONDITION_VARIABLE_INIT            {INIT_ONCE_STATIC_INIT, {INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE}, 0}
#define CONDITION_VARIABLE_LOCKMODE_SHARED 0x01

typedef struct _SRWLOCK
{
    // highest bit: reader lock (0=free, 1=locked)
    // second-highest bit: global lock (0=free, 1=locked)
    // remainder: blocking readers
    DWORD_PTR Ptr;
} SRWLOCK, *PSRWLOCK;

#define SRWLOCK_INIT {0}

#define FILE_BEGIN               0
#define FILE_CURRENT             1
#define FILE_END                 2
#define INVALID_SET_FILE_POINTER ((DWORD) - 1)
#define INVALID_FILE_SIZE        ((DWORD) - 1)

#define FILE_FLAG_WRITE_THROUGH    0x80000000
#define FILE_FLAG_OVERLAPPED       0x40000000
#define FILE_FLAG_NO_BUFFERING     0x20000000
#define FILE_FLAG_RANDOM_ACCESS    0x10000000
#define FILE_FLAG_SEQUENTIAL_SCAN  0x08000000
#define FILE_FLAG_DELETE_ON_CLOSE  0x04000000
#define FILE_FLAG_BACKUP_SEMANTICS 0x02000000
#define FILE_FLAG_POSIX_SEMANTICS  0x01000000

typedef enum _GET_FILEEX_INFO_LEVELS
{
    GetFileExInfoStandard,
    GetFileExMaxInfoLevel
} GET_FILEEX_INFO_LEVELS;

typedef struct _WIN32_FILE_ATTRIBUTE_DATA
{
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
} WIN32_FILE_ATTRIBUTE_DATA, *LPWIN32_FILE_ATTRIBUTE_DATA;

typedef struct _SECURITY_ATTRIBUTES
{
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _WIN32_FIND_DATAA
{
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
#define WIN32_FIND_DATA   WIN32_FIND_DATAA
#define PWIN32_FIND_DATA  PWIN32_FIND_DATAA
#define LPWIN32_FIND_DATA LPWIN32_FIND_DATAA
#endif

VOID WINAPI ZeroMemory (PVOID Destination, SIZE_T length);

DWORD GetLastError (void);
void SetLastError (DWORD error);

void WINAPI OutputDebugStringA (LPCTSTR lpOutputString);

BOOL IsBadWritePtr (LPVOID lp, UINT_PTR ucb);

BOOL GetOverlappedResult (HANDLE hFile, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait);

static inline PVOID SecureZeroMemory (PVOID ptr, SIZE_T cnt)
{
    volatile char *cur_ptr = (volatile char *)ptr;
    volatile char *end_ptr = (volatile char *)ptr + cnt;

    while (cur_ptr < end_ptr) {
        *cur_ptr = 0;
        cur_ptr++;
    }

    return ptr;
}

#ifndef UNICODE
#define OutputDebugString OutputDebugStringA
#else
#error nxdk does not support the Unicode API
#endif

#ifdef __cplusplus
}
#endif

#endif
