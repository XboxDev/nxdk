#include <fileapi.h>
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
