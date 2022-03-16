// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2022 Stefan Schmidt
// SPDX-FileCopyrightText: 2021 Erik Abair

#include <fileapi.h>
#include <handleapi.h>
#include <winbase.h>
#include <winerror.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <xboxkrnl/xboxkrnl.h>

DWORD GetFileAttributesA (LPCSTR lpFileName)
{
    NTSTATUS status;
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    FILE_NETWORK_OPEN_INFORMATION openInfo;

    assert(lpFileName != NULL);
    RtlInitAnsiString(&path, lpFileName);

    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtQueryFullAttributesFile(&objectAttributes, &openInfo);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return INVALID_FILE_ATTRIBUTES;
    }

    return openInfo.FileAttributes;
}

BOOL GetFileAttributesExA (LPCSTR lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation)
{
    NTSTATUS status;
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    FILE_NETWORK_OPEN_INFORMATION openInfo;

    assert(fInfoLevelId == GetFileExInfoStandard);

    assert(lpFileName != NULL);
    RtlInitAnsiString(&path, lpFileName);

    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtQueryFullAttributesFile(&objectAttributes, &openInfo);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    LPWIN32_FILE_ATTRIBUTE_DATA fileInfo = lpFileInformation;
    fileInfo->dwFileAttributes = openInfo.FileAttributes;
    fileInfo->ftCreationTime.dwHighDateTime = openInfo.CreationTime.HighPart;
    fileInfo->ftCreationTime.dwLowDateTime = openInfo.CreationTime.LowPart;
    fileInfo->ftLastAccessTime.dwHighDateTime = openInfo.LastAccessTime.HighPart;
    fileInfo->ftLastAccessTime.dwLowDateTime = openInfo.LastAccessTime.LowPart;
    fileInfo->ftLastWriteTime.dwHighDateTime = openInfo.LastWriteTime.HighPart;
    fileInfo->ftLastWriteTime.dwLowDateTime = openInfo.LastWriteTime.LowPart;
    fileInfo->nFileSizeHigh = openInfo.EndOfFile.HighPart;
    fileInfo->nFileSizeLow = openInfo.EndOfFile.LowPart;
    return TRUE;
}

BOOL SetFileAttributesA (LPCSTR lpFileName, DWORD dwFileAttributes)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_BASIC_INFORMATION fileBasicInfo;

    assert(lpFileName != NULL);
    RtlInitAnsiString(&path, lpFileName);

    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtOpenFile(&handle, FILE_WRITE_ATTRIBUTES | SYNCHRONIZE, &objectAttributes, &ioStatusBlock, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_SYNCHRONOUS_IO_NONALERT);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    // Setting the times to zero makes the kernel preserve their current values
    fileBasicInfo.CreationTime.QuadPart = 0;
    fileBasicInfo.LastAccessTime.QuadPart = 0;
    fileBasicInfo.LastWriteTime.QuadPart = 0;
    fileBasicInfo.ChangeTime.QuadPart = 0;

    fileBasicInfo.FileAttributes = dwFileAttributes | FILE_ATTRIBUTE_NORMAL;

    status = NtSetInformationFile(handle, &ioStatusBlock, &fileBasicInfo, sizeof(fileBasicInfo), FileBasicInformation);
    NtClose(handle);

    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    return TRUE;
}

BOOL GetFileTime (HANDLE hFile, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_NETWORK_OPEN_INFORMATION openInfo;

    status = NtQueryInformationFile(hFile, &ioStatusBlock, &openInfo, sizeof(openInfo), FileNetworkOpenInformation);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    if (lpCreationTime) {
        lpCreationTime->dwLowDateTime = openInfo.CreationTime.LowPart;
        lpCreationTime->dwHighDateTime = openInfo.CreationTime.HighPart;
    }

    if (lpLastAccessTime) {
        lpLastAccessTime->dwLowDateTime = openInfo.LastAccessTime.LowPart;
        lpLastAccessTime->dwHighDateTime = openInfo.LastAccessTime.HighPart;
    }

    if (lpLastWriteTime) {
        lpLastWriteTime->dwLowDateTime = openInfo.LastWriteTime.LowPart;
        lpLastWriteTime->dwHighDateTime = openInfo.LastWriteTime.HighPart;
    }

    return TRUE;
}

BOOL SetFileTime (HANDLE hFile, const FILETIME *lpCreationTime, const FILETIME *lpLastAccessTime, const FILETIME *lpLastWriteTime)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_BASIC_INFORMATION fileBasicInfo;

    ZeroMemory(&fileBasicInfo, sizeof(fileBasicInfo));

    if (lpCreationTime) {
        fileBasicInfo.CreationTime.LowPart = lpCreationTime->dwLowDateTime;
        fileBasicInfo.CreationTime.HighPart = lpCreationTime->dwHighDateTime;
    }

    if (lpLastAccessTime) {
        fileBasicInfo.LastAccessTime.LowPart = lpLastAccessTime->dwLowDateTime;
        fileBasicInfo.LastAccessTime.HighPart = lpLastAccessTime->dwHighDateTime;
    }

    if (lpLastWriteTime) {
        fileBasicInfo.LastWriteTime.LowPart = lpLastWriteTime->dwLowDateTime;
        fileBasicInfo.LastWriteTime.HighPart = lpLastWriteTime->dwHighDateTime;
    }

    status = NtSetInformationFile(hFile, &ioStatusBlock, &fileBasicInfo, sizeof(fileBasicInfo), FileBasicInformation);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    return TRUE;
}

static NTSTATUS DeleteHandle (HANDLE handle)
{
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_DISPOSITION_INFORMATION dispositionInformation;
    dispositionInformation.DeleteFile = TRUE;

    return NtSetInformationFile(handle, &ioStatusBlock, &dispositionInformation, sizeof(dispositionInformation), FileDispositionInformation);
}

BOOL DeleteFileA (LPCSTR lpFileName)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;

    assert(lpFileName != NULL);
    RtlInitAnsiString(&path, lpFileName);

    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtOpenFile(&handle, DELETE | SYNCHRONIZE, &objectAttributes, &ioStatusBlock, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = DeleteHandle(handle);

    if (!NT_SUCCESS(status)) {
        NtClose(handle);
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = NtClose(handle);
    if (NT_SUCCESS(status)) {
        return TRUE;
    } else {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }
}

BOOL RemoveDirectoryA (LPCSTR lpPathName)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;

    assert(lpPathName != NULL);
    RtlInitAnsiString(&path, lpPathName);

    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtOpenFile(&handle, DELETE | SYNCHRONIZE, &objectAttributes, &ioStatusBlock, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = DeleteHandle(handle);

    if (!NT_SUCCESS(status)) {
        NtClose(handle);
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = NtClose(handle);
    if (NT_SUCCESS(status)) {
        return TRUE;
    } else {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }
}

BOOL CreateDirectoryA (LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;

    RtlInitAnsiString(&path, lpPathName);

    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtCreateFile(&handle, FILE_LIST_DIRECTORY | SYNCHRONIZE, &objectAttributes, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_CREATE, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = NtClose(handle);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    } else {
        return TRUE;
    }
}

BOOL MoveFileA (LPCSTR lpExistingFileName, LPCSTR lpNewFileName)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_RENAME_INFORMATION renameInfo;

    RtlInitAnsiString(&path, lpExistingFileName);

    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtOpenFile(&handle, DELETE | SYNCHRONIZE, &objectAttributes, &ioStatusBlock, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    renameInfo.ReplaceIfExists = FALSE;
    renameInfo.RootDirectory = ObDosDevicesDirectory();
    RtlInitAnsiString(&renameInfo.FileName, lpNewFileName);

    status = NtSetInformationFile(handle, &ioStatusBlock, &renameInfo, sizeof(renameInfo), FileRenameInformation);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = NtClose(handle);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    } else {
        return TRUE;
    }
}

BOOL CopyFileA (LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)
{
    NTSTATUS status;
    HANDLE sourceHandle;
    HANDLE targetHandle = INVALID_HANDLE_VALUE;
    ANSI_STRING targetPath;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_BASIC_INFORMATION fileBasicInformation;
    FILE_NETWORK_OPEN_INFORMATION networkOpenInformation;
    LPVOID readBuffer = NULL;
    SIZE_T readBufferRegionSize = 64 * 1024;
    DWORD bytesRead;

    sourceHandle = CreateFile(
        lpExistingFileName,
        FILE_GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);
    if (sourceHandle == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    status = NtQueryInformationFile(
        sourceHandle,
        &ioStatusBlock,
        &networkOpenInformation,
        sizeof(networkOpenInformation),
        FileNetworkOpenInformation);
    if (!NT_SUCCESS(status)) {
        NtClose(sourceHandle);
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    RtlInitAnsiString(&targetPath, lpNewFileName);
    InitializeObjectAttributes(&objectAttributes, &targetPath, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtCreateFile(
            &targetHandle,
            FILE_GENERIC_WRITE,
            &objectAttributes,
            &ioStatusBlock,
            &networkOpenInformation.AllocationSize,
            networkOpenInformation.FileAttributes,
            0,
            bFailIfExists ? FILE_CREATE : FILE_SUPERSEDE,
            FILE_SYNCHRONOUS_IO_NONALERT | FILE_NON_DIRECTORY_FILE | FILE_SEQUENTIAL_ONLY);
    if (!NT_SUCCESS(status)) {
        NtClose(sourceHandle);
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = NtAllocateVirtualMemory(&readBuffer,
                                      0,
                                      &readBufferRegionSize,
                                      MEM_RESERVE | MEM_COMMIT,
                                      PAGE_READWRITE);
    if (!NT_SUCCESS(status)) {
        NtClose(sourceHandle);
        NtClose(targetHandle);
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    while (TRUE) {
        const BYTE *bufferPos = readBuffer;
        if (!ReadFile(sourceHandle, readBuffer, readBufferRegionSize, &bytesRead, NULL)) {
            NtFreeVirtualMemory(&readBuffer, &readBufferRegionSize, MEM_RELEASE);
            DeleteHandle(targetHandle);
            NtClose(sourceHandle);
            NtClose(targetHandle);
            return FALSE;
        }

        if (!bytesRead) {
            break;
        }

        while (bytesRead > 0) {
            DWORD bytesWritten = 0;
            if (!WriteFile(targetHandle, bufferPos, bytesRead, &bytesWritten, NULL)) {
                NtFreeVirtualMemory(&readBuffer, &readBufferRegionSize, MEM_RELEASE);
                DeleteHandle(targetHandle);
                NtClose(sourceHandle);
                NtClose(targetHandle);
                return FALSE;
            }
            bytesRead -= bytesWritten;
            bufferPos += bytesWritten;
        }
    }

    status = NtFreeVirtualMemory(&readBuffer, &readBufferRegionSize, MEM_RELEASE);
    assert(NT_SUCCESS(status));

    RtlZeroMemory(&fileBasicInformation, sizeof(fileBasicInformation));
    fileBasicInformation.LastWriteTime = networkOpenInformation.LastWriteTime;
    fileBasicInformation.FileAttributes = networkOpenInformation.FileAttributes;
    status = NtSetInformationFile(
            targetHandle,
            &ioStatusBlock,
            &fileBasicInformation,
            sizeof(fileBasicInformation),
            FileBasicInformation);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        NtClose(sourceHandle);
        NtClose(targetHandle);
        return FALSE;
    }

    status = NtClose(sourceHandle);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        NtClose(targetHandle);
        return FALSE;
    }

    status = NtClose(targetHandle);
    if (!NT_SUCCESS(status)) {
      SetLastError(RtlNtStatusToDosError(status));
      return FALSE;
    }
    return TRUE;
}

BOOL GetDiskFreeSpaceExA (LPCSTR lpDirectoryName, PULARGE_INTEGER lpFreeBytesAvailableToCaller, PULARGE_INTEGER lpTotalNumberOfBytes, PULARGE_INTEGER lpTotalNumberOfFreeBytes)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_FS_SIZE_INFORMATION fsSizeInfo;

    assert(lpDirectoryName);

    RtlInitAnsiString(&path, lpDirectoryName);
    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtOpenFile(&handle, FILE_LIST_DIRECTORY | SYNCHRONIZE, &objectAttributes, &ioStatusBlock, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_SYNCHRONOUS_IO_NONALERT | FILE_DIRECTORY_FILE | FILE_OPEN_FOR_FREE_SPACE_QUERY);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            SetLastError(ERROR_PATH_NOT_FOUND);
        }
        return FALSE;
    }

    status = NtQueryVolumeInformationFile(handle, &ioStatusBlock, &fsSizeInfo, sizeof(fsSizeInfo), FileFsSizeInformation);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = NtClose(handle);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    ULONGLONG bytesPerAllocUnit = fsSizeInfo.BytesPerSector * fsSizeInfo.SectorsPerAllocationUnit;
    ULONGLONG totalBytes = bytesPerAllocUnit * fsSizeInfo.TotalAllocationUnits.QuadPart;
    ULONGLONG freeBytes = bytesPerAllocUnit * fsSizeInfo.AvailableAllocationUnits.QuadPart;

    if (lpFreeBytesAvailableToCaller) {
        lpFreeBytesAvailableToCaller->QuadPart = freeBytes;
    }
    if (lpTotalNumberOfBytes) {
        lpTotalNumberOfBytes->QuadPart = totalBytes;
    }
    if (lpTotalNumberOfFreeBytes) {
        lpTotalNumberOfFreeBytes->QuadPart = freeBytes;
    }

    return TRUE;
}

BOOL GetDiskFreeSpaceA (LPCSTR lpRootPathName, LPDWORD lpSectorsPerCluster, LPDWORD lpBytesPerSector, LPDWORD lpNumberOfFreeClusters, LPDWORD lpTotalNumberOfClusters)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_FS_SIZE_INFORMATION fsSizeInfo;

    assert(lpRootPathName);

    RtlInitAnsiString(&path, lpRootPathName);
    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtOpenFile(&handle, FILE_LIST_DIRECTORY | SYNCHRONIZE, &objectAttributes, &ioStatusBlock, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_SYNCHRONOUS_IO_NONALERT | FILE_DIRECTORY_FILE | FILE_OPEN_FOR_FREE_SPACE_QUERY);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            SetLastError(ERROR_PATH_NOT_FOUND);
        }
        return FALSE;
    }

    status = NtQueryVolumeInformationFile(handle, &ioStatusBlock, &fsSizeInfo, sizeof(fsSizeInfo), FileFsSizeInformation);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = NtClose(handle);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    if (lpSectorsPerCluster) {
        *lpSectorsPerCluster = fsSizeInfo.SectorsPerAllocationUnit;
    }
    if (lpBytesPerSector) {
        *lpBytesPerSector = fsSizeInfo.BytesPerSector;
    }
    if (lpNumberOfFreeClusters) {
        *lpNumberOfFreeClusters = fsSizeInfo.AvailableAllocationUnits.HighPart ? 0xFFFFFFFF : fsSizeInfo.AvailableAllocationUnits.LowPart;
    }
    if (lpTotalNumberOfClusters) {
        *lpTotalNumberOfClusters = fsSizeInfo.TotalAllocationUnits.HighPart ? 0xFFFFFFFF : fsSizeInfo.TotalAllocationUnits.LowPart;
    }

    return TRUE;
}

DWORD GetLogicalDrives (VOID)
{
    DWORD result = 0;
    NTSTATUS status;
    ANSI_STRING path;
    HANDLE handle;
    OBJECT_ATTRIBUTES attributes;
    struct {
        OBJECT_DIRECTORY_INFORMATION objDirInfo;
        CHAR filenameBuf[2];
    } objDirInfoBuf;

    InitializeObjectAttributes(&attributes, NULL, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);
    status = NtOpenDirectoryObject(&handle, &attributes);

    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return 0;
    }

    ULONG index = 0;
    while (true) {
        POBJECT_DIRECTORY_INFORMATION objDirInfo = (POBJECT_DIRECTORY_INFORMATION)&objDirInfoBuf;
        status = NtQueryDirectoryObject(handle, objDirInfo, sizeof(objDirInfoBuf), (index == 0), &index, NULL);

        if (status == STATUS_NO_MORE_ENTRIES) {
            break;
        } else if (status == STATUS_BUFFER_TOO_SMALL) {
            // If the object name is too long, we're not interested in it anyway
            continue;
        } else if (!NT_SUCCESS(status)) {
            NtClose(handle);
            SetLastError(RtlNtStatusToDosError(status));
            return 0;
        }

        // Only allow entries like "C:" or "E:", ignore stuff like "CdRom0:"
        if (objDirInfo->Name.Length != 2 || objDirInfo->Name.Buffer[1] != ':') {
            continue;
        }

        if (isalpha(objDirInfo->Name.Buffer[0])) {
            result |= 1 << (toupper(objDirInfo->Name.Buffer[0]) - 'A');
        }
    }

    NtClose(handle);
    SetLastError(ERROR_SUCCESS);
    return result;
}

DWORD GetLogicalDriveStringsA (DWORD nBufferLength, LPSTR lpBuffer)
{
    DWORD driveBitmask = GetLogicalDrives();
    if (driveBitmask == 0 && GetLastError() != ERROR_SUCCESS) {
        return 0;
    }

    DWORD requiredBufLength = 0;
    for (int bit = 0; bit < 26; bit++) {
        if (driveBitmask & (1 << bit)) {
            requiredBufLength += 4;
        }
    }

    if (nBufferLength == 0 || nBufferLength < requiredBufLength) {
        return requiredBufLength+1;
    }

    for (int bit = 0; bit < 26; bit++) {
        if (driveBitmask & (1 << bit)) {
            *lpBuffer++ = 'A' + bit;
            *lpBuffer++ = ':';
            *lpBuffer++ = '\\';
            *lpBuffer++ = '\0';
        }
    }
    *lpBuffer = '\0';
    return requiredBufLength;
}
