#include <fileapi.h>
#include <assert.h>
#include <xboxkrnl/xboxkrnl.h>

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
