#include <fileapi.h>
#include <winerror.h>
#include <assert.h>
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

BOOL DeleteFileA (LPCTSTR lpFileName)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING path;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_DISPOSITION_INFORMATION dispositionInformation;

    assert(lpFileName != NULL);
    RtlInitAnsiString(&path, lpFileName);

    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtOpenFile(&handle, DELETE | SYNCHRONIZE, &objectAttributes, &ioStatusBlock, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    dispositionInformation.DeleteFile = TRUE;

    status = NtSetInformationFile(handle, &ioStatusBlock, &dispositionInformation, sizeof(dispositionInformation), FileDispositionInformation);

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
    FILE_DISPOSITION_INFORMATION dispositionInformation;

    assert(lpPathName != NULL);
    RtlInitAnsiString(&path, lpPathName);

    InitializeObjectAttributes(&objectAttributes, &path, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtOpenFile(&handle, DELETE | SYNCHRONIZE, &objectAttributes, &ioStatusBlock, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    dispositionInformation.DeleteFile = TRUE;

    status = NtSetInformationFile(handle, &ioStatusBlock, &dispositionInformation, sizeof(dispositionInformation), FileDispositionInformation);
    if (!NT_SUCCESS(status)) {
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

BOOL MoveFileA (LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName)
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
