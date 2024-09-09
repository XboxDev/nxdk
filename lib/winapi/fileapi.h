// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2021 Erik Abair

#ifndef __FILEAPI_H__
#define __FILEAPI_H__

#include <winbase.h>
#include <windef.h>
#include <winnt.h>

#ifdef __cplusplus
extern "C" {
#endif

DWORD GetFileAttributesA (LPCSTR lpFileName);
BOOL GetFileAttributesExA (LPCSTR lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation);
BOOL SetFileAttributesA (LPCSTR lpFileName, DWORD dwFileAttributes);

BOOL GetFileTime (HANDLE hFile, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime);
BOOL SetFileTime (HANDLE hFile, const FILETIME *lpCreationTime, const FILETIME *lpLastAccessTime, const FILETIME *lpLastWriteTime);

#define CREATE_NEW        1
#define CREATE_ALWAYS     2
#define OPEN_EXISTING     3
#define OPEN_ALWAYS       4
#define TRUNCATE_EXISTING 5

HANDLE CreateFileA (LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
BOOL ReadFile (HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
BOOL WriteFile (HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
BOOL SetEndOfFile (HANDLE hFile);
DWORD SetFilePointer (HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);
BOOL SetFilePointerEx (HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod);

DWORD GetFileSize (HANDLE hFile, LPDWORD lpFileSizeHigh);
BOOL GetFileSizeEx (HANDLE hFile, PLARGE_INTEGER lpFileSize);

HANDLE FindFirstFileA (LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
BOOL FindNextFileA (HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
BOOL FindClose (HANDLE hFindFile);

BOOL DeleteFileA (LPCSTR lpFileName);
BOOL RemoveDirectoryA (LPCSTR lpPathName);
BOOL CreateDirectoryA (LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
BOOL MoveFileA (LPCSTR lpExistingFileName, LPCSTR lpNewFileName);
BOOL CopyFileA (LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists);

BOOL GetDiskFreeSpaceExA (LPCSTR lpDirectoryName, PULARGE_INTEGER lpFreeBytesAvailableToCaller, PULARGE_INTEGER lpTotalNumberOfBytes, PULARGE_INTEGER lpTotalNumberOfFreeBytes);
BOOL GetDiskFreeSpaceA (LPCSTR lpRootPathName, LPDWORD lpSectorsPerCluster, LPDWORD lpBytesPerSector, LPDWORD lpNumberOfFreeClusters, LPDWORD lpTotalNumberOfClusters);
DWORD GetLogicalDrives (VOID);
DWORD GetLogicalDriveStringsA (DWORD nBufferLength, LPSTR lpBuffer);

#ifndef UNICODE
#define GetFileAttributes      GetFileAttributesA
#define GetFileAttributesEx    GetFileAttributesExA
#define SetFileAttributes      SetFileAttributesA
#define CreateFile             CreateFileA
#define FindFirstFile          FindFirstFileA
#define FindNextFile           FindNextFileA
#define DeleteFile(...)        DeleteFileA(__VA_ARGS__)
#define RemoveDirectory(...)   RemoveDirectoryA(__VA_ARGS__)
#define CreateDirectory(...)   CreateDirectoryA(__VA_ARGS__)
#define MoveFile(...)          MoveFileA(__VA_ARGS__)
#define CopyFile(...)          CopyFileA(__VA_ARGS__)
#define GetDiskFreeSpaceEx     GetDiskFreeSpaceExA
#define GetDiskFreeSpace       GetDiskFreeSpaceA
#define GetLogicalDriveStrings GetLogicalDriveStringsA
#else
#error nxdk does not support the Unicode API
#endif

#ifdef __cplusplus
}
#endif

#endif
