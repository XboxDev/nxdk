#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <winbase.h>
#include <xboxkrnl/xboxkrnl.h>
#include <winerror.h>
#include <fileapi.h>

struct FileInfo {
    FILE_DIRECTORY_INFORMATION dirInfo;
    // Reserve path buffer minus the null-terminator and the first byte provided by dirInfo
    char filename[MAX_PATH-2];
};

static void dirtofind (FILE_DIRECTORY_INFORMATION *dirInfo, LPWIN32_FIND_DATAA lpFindFileData)
{
    lpFindFileData->dwFileAttributes = dirInfo->FileAttributes;
    lpFindFileData->ftCreationTime.dwLowDateTime = dirInfo->CreationTime.LowPart;
    lpFindFileData->ftCreationTime.dwHighDateTime = dirInfo->CreationTime.HighPart;
    lpFindFileData->ftLastAccessTime.dwLowDateTime  = dirInfo->LastAccessTime.LowPart;
    lpFindFileData->ftLastAccessTime.dwHighDateTime = dirInfo->LastAccessTime.HighPart;
    lpFindFileData->ftLastWriteTime.dwLowDateTime  = dirInfo->LastWriteTime.LowPart;
    lpFindFileData->ftLastWriteTime.dwHighDateTime = dirInfo->LastWriteTime.HighPart;
    lpFindFileData->nFileSizeHigh = dirInfo->EndOfFile.HighPart;
    lpFindFileData->nFileSizeLow = dirInfo->EndOfFile.LowPart;
    memcpy(lpFindFileData->cFileName, dirInfo->FileName, dirInfo->FileNameLength);
    lpFindFileData->cFileName[dirInfo->FileNameLength] = '\0';
    lpFindFileData->cAlternateFileName[0] = '\0';
}

HANDLE FindFirstFileA (LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
    NTSTATUS status;
    ANSI_STRING dirPath;
    ANSI_STRING mask;
    IO_STATUS_BLOCK ioStatusBlock;
    OBJECT_ATTRIBUTES attributes;
    struct FileInfo fileInformation;
    HANDLE handle;
    size_t maskOffset;

    assert(strlen(lpFileName) < MAX_PATH);

    RtlInitAnsiString(&dirPath, lpFileName);

    for (maskOffset = dirPath.Length; maskOffset > 0; maskOffset--) {
        if (dirPath.Buffer[maskOffset - 1] == '\\')
            break;
    }

    mask.Buffer = dirPath.Buffer + maskOffset;
    mask.Length = dirPath.Length - maskOffset;
    mask.MaximumLength = mask.Length;

    dirPath.Length = maskOffset;
    dirPath.MaximumLength = dirPath.Length;

    if (dirPath.Length == 0 || mask.Length == 0) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return INVALID_HANDLE_VALUE;
    }

    // Ensure that "*.*" lists all files, just like on Windows
    if (mask.Length == 3 && memcmp(mask.Buffer, "*.*", 3)) {
        mask.Length = 0;
    }

    InitializeObjectAttributes(&attributes, &dirPath, OBJ_CASE_INSENSITIVE, ObDosDevicesDirectory(), NULL);

    status = NtOpenFile(&handle, FILE_LIST_DIRECTORY | SYNCHRONIZE, &attributes, &ioStatusBlock, FILE_SHARE_READ, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);

    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return INVALID_HANDLE_VALUE;
    }

    RtlZeroMemory(&fileInformation, sizeof(fileInformation));
    status = NtQueryDirectoryFile(handle, NULL, NULL, NULL, &ioStatusBlock, &fileInformation, sizeof(fileInformation), FileDirectoryInformation, &mask, TRUE);

    if (!NT_SUCCESS(status)) {
        NtClose(handle);
        SetLastError(RtlNtStatusToDosError(status));
        return INVALID_HANDLE_VALUE;
    }

    dirtofind(&fileInformation.dirInfo, lpFindFileData);

    return handle;
}

BOOL FindNextFileA (HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;
    struct FileInfo fileInformation;

    RtlZeroMemory(&fileInformation, sizeof(fileInformation));
    status = NtQueryDirectoryFile(hFindFile, NULL, NULL, NULL, &ioStatusBlock, &fileInformation, sizeof(fileInformation), FileDirectoryInformation, NULL, FALSE);
    if (status == STATUS_NO_MORE_FILES) {
        SetLastError(ERROR_NO_MORE_FILES);
        return FALSE;
    } else if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    dirtofind(&fileInformation.dirInfo, lpFindFileData);

    return TRUE;
}

BOOL FindClose (HANDLE hFindFile)
{
    NTSTATUS status = NtClose(hFindFile);

    if (NT_SUCCESS(status)) {
        return TRUE;
    }

    SetLastError(RtlNtStatusToDosError(status));
    return FALSE;
}
