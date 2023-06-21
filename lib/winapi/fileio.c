// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2021 Stefan Schmidt

#include <fileapi.h>
#include <winerror.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <xboxkrnl/xboxkrnl.h>

HANDLE CreateFileA (LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING path;
    IO_STATUS_BLOCK ioStatusBlock;
    OBJECT_ATTRIBUTES attributes;
    ULONG creationDisposition;
    ULONG creationFlags = 0;

    // no extended attributes on the Xbox
    assert(hTemplateFile == NULL);

    if (!lpFileName || !lpFileName[0]) {
        SetLastError(ERROR_PATH_NOT_FOUND);
        return INVALID_HANDLE_VALUE;
    }
    assert(strlen(lpFileName) < MAX_PATH);
    RtlInitAnsiString(&path, lpFileName);

    InitializeObjectAttributes(&attributes, &path, 0, ObDosDevicesDirectory(), NULL);

    if (!(dwFlagsAndAttributes & FILE_FLAG_POSIX_SEMANTICS)) {
        attributes.Attributes |= OBJ_CASE_INSENSITIVE;
    }

    switch (dwCreationDisposition) {
        case CREATE_NEW:
            creationDisposition = FILE_CREATE;
            break;
        case CREATE_ALWAYS:
            creationDisposition = FILE_OVERWRITE_IF;
            break;
        case OPEN_EXISTING:
            creationDisposition = FILE_OPEN;
            break;
        case OPEN_ALWAYS:
            creationDisposition = FILE_OPEN_IF;
            break;
        case TRUNCATE_EXISTING:
            creationDisposition = FILE_OVERWRITE;
            if (!(dwDesiredAccess & GENERIC_WRITE)) {
                SetLastError(ERROR_INVALID_PARAMETER);
                return INVALID_HANDLE_VALUE;
            }
            break;
        default:
            SetLastError(ERROR_INVALID_PARAMETER);
            return INVALID_HANDLE_VALUE;
    }

    if (dwFlagsAndAttributes & FILE_FLAG_DELETE_ON_CLOSE) {
        creationFlags |= FILE_DELETE_ON_CLOSE;
        dwDesiredAccess |= DELETE;
    }
    creationFlags |= (dwFlagsAndAttributes & FILE_FLAG_WRITE_THROUGH) ? FILE_WRITE_THROUGH : 0;
    creationFlags |= (dwFlagsAndAttributes & FILE_FLAG_NO_BUFFERING) ? FILE_NO_INTERMEDIATE_BUFFERING : 0;
    creationFlags |= (dwFlagsAndAttributes & FILE_FLAG_OVERLAPPED) ? 0 : FILE_SYNCHRONOUS_IO_NONALERT;
    creationFlags |= (dwFlagsAndAttributes & FILE_FLAG_RANDOM_ACCESS) ? FILE_RANDOM_ACCESS : 0;
    creationFlags |= (dwFlagsAndAttributes & FILE_FLAG_SEQUENTIAL_SCAN) ? FILE_SEQUENTIAL_ONLY : 0;
    creationFlags |= (dwFlagsAndAttributes & FILE_FLAG_BACKUP_SEMANTICS) ? FILE_OPEN_FOR_BACKUP_INTENT : FILE_NON_DIRECTORY_FILE;

    status = NtCreateFile(&handle, dwDesiredAccess | FILE_READ_ATTRIBUTES | SYNCHRONIZE, &attributes, &ioStatusBlock, NULL, dwFlagsAndAttributes, dwShareMode, creationDisposition, creationFlags);

    if (!NT_SUCCESS(status)) {
        if (status == STATUS_OBJECT_NAME_COLLISION) {
            SetLastError(ERROR_FILE_EXISTS);
        } else if (status == STATUS_FILE_IS_A_DIRECTORY) {
            if (lpFileName[path.Length-1] == '\\') {
                SetLastError(ERROR_PATH_NOT_FOUND);
            } else {
                SetLastError(ERROR_ACCESS_DENIED);
            }
        } else {
            SetLastError(RtlNtStatusToDosError(status));
        }

        return INVALID_HANDLE_VALUE;
    }

    if (((dwCreationDisposition == CREATE_ALWAYS) && (ioStatusBlock.Information == FILE_OVERWRITTEN)) || ((dwCreationDisposition == OPEN_ALWAYS) && (ioStatusBlock.Information == FILE_OPENED))) {
        SetLastError(ERROR_ALREADY_EXISTS);
    } else {
        SetLastError(ERROR_SUCCESS);
    }

    return handle;
}

BOOL ReadFile (HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;

    if (lpOverlapped == NULL) {
        // A null pointer makes the code crash on Windows, we'd rather catch it
        assert(lpNumberOfBytesRead);
    }

    if (lpNumberOfBytesRead) {
        *lpNumberOfBytesRead = 0;
    }

    if (lpOverlapped) {
        LARGE_INTEGER overlappedOffset = {
            .LowPart = lpOverlapped->Offset,
            .HighPart = lpOverlapped->OffsetHigh,
        };

        // Internal contains the status code for the I/O request. Set to STATUS_PENDING at the start of the request.
        lpOverlapped->Internal = (DWORD)STATUS_PENDING;
        // InternalHigh contains the actual number of bytes transferred for the I/O request
        lpOverlapped->InternalHigh = 0;

        status = NtReadFile(hFile, lpOverlapped->hEvent, NULL, NULL, (PIO_STATUS_BLOCK)&lpOverlapped->Internal,
                            lpBuffer, nNumberOfBytesToRead, &overlappedOffset);

        // The read can finish immediately. Handle this case
        if (NT_SUCCESS(status) && status != STATUS_PENDING) {
            if (lpNumberOfBytesRead) {
                *lpNumberOfBytesRead = (DWORD)lpOverlapped->InternalHigh;
            }
            return TRUE;
        } else if (status == STATUS_END_OF_FILE) {
            *lpNumberOfBytesRead = 0;
            return TRUE;
        }
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = NtReadFile(hFile, NULL, NULL, NULL, &ioStatusBlock, lpBuffer, nNumberOfBytesToRead, NULL);

    if (status == STATUS_PENDING) {
        status = NtWaitForSingleObject(hFile, FALSE, NULL);
        if (NT_SUCCESS(status)) {
            status = ioStatusBlock.Status;
        }
    }

    if (NT_SUCCESS(status)) {
        *lpNumberOfBytesRead = ioStatusBlock.Information;
        return TRUE;
    } else if (status == STATUS_END_OF_FILE) {
        *lpNumberOfBytesRead = 0;
        return TRUE;
    } else {
        if (NT_WARNING(status)) {
            *lpNumberOfBytesRead = ioStatusBlock.Information;
        }
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }
}

BOOL WriteFile (HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;

    if (lpOverlapped == NULL) {
        // A null pointer makes the code crash on Windows, we'd rather catch it
        assert(lpNumberOfBytesWritten);
    }

    if (lpNumberOfBytesWritten) {
        *lpNumberOfBytesWritten = 0;
    }

    if (lpOverlapped) {
        LARGE_INTEGER overlappedOffset = {
            .LowPart = lpOverlapped->Offset,
            .HighPart = lpOverlapped->OffsetHigh,
        };

        // Internal contains the status code for the I/O request. Set to STATUS_PENDING at the start of the request
        lpOverlapped->Internal = (DWORD)STATUS_PENDING;
        // InternalHigh contains the actual number of bytes transferred for the I/O request
        lpOverlapped->InternalHigh = 0;

        status = NtWriteFile(hFile, lpOverlapped->hEvent, NULL, NULL, (PIO_STATUS_BLOCK)&lpOverlapped->Internal,
                            (PVOID)lpBuffer, nNumberOfBytesToWrite, &overlappedOffset);

        // The write can finish immediately. Handle this case
        if (NT_SUCCESS(status) && status != STATUS_PENDING) {
            if (lpNumberOfBytesWritten) {
                *lpNumberOfBytesWritten = (DWORD)lpOverlapped->InternalHigh;
            }
            return TRUE;
        }
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = NtWriteFile(hFile, NULL, NULL, NULL, &ioStatusBlock, (PVOID)lpBuffer, nNumberOfBytesToWrite, NULL);

    if (status == STATUS_PENDING) {
        status = NtWaitForSingleObject(hFile, FALSE, NULL);
        if (NT_SUCCESS(status)) {
            status = ioStatusBlock.Status;
        }
    }

    if (NT_SUCCESS(status)) {
        *lpNumberOfBytesWritten = ioStatusBlock.Information;
        return TRUE;
    } else {
        if (NT_WARNING(status)) {
            *lpNumberOfBytesWritten = ioStatusBlock.Information;
        }
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }
}

BOOL SetEndOfFile (HANDLE hFile)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_POSITION_INFORMATION positionInfo;
    FILE_END_OF_FILE_INFORMATION eofInfo;
    FILE_ALLOCATION_INFORMATION allocationInfo;

    status = NtQueryInformationFile(hFile, &ioStatusBlock, &positionInfo, sizeof(positionInfo), FilePositionInformation);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    eofInfo.EndOfFile = positionInfo.CurrentByteOffset;
    allocationInfo.AllocationSize = positionInfo.CurrentByteOffset;

    status = NtSetInformationFile(hFile, &ioStatusBlock, &eofInfo, sizeof(eofInfo), FileEndOfFileInformation);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    status = NtSetInformationFile(hFile, &ioStatusBlock, &allocationInfo, sizeof(allocationInfo), FileAllocationInformation);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    return TRUE;
}

DWORD SetFilePointer (HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_POSITION_INFORMATION positionInfo;
    LARGE_INTEGER distance;

    if (lpDistanceToMoveHigh) {
        distance.HighPart = *lpDistanceToMoveHigh;
        distance.LowPart = lDistanceToMove;
    } else {
        distance.QuadPart = lDistanceToMove;
    }

    switch (dwMoveMethod) {
        default:
            assert(false);
        case FILE_BEGIN:
            positionInfo.CurrentByteOffset = distance;
            break;
        case FILE_CURRENT:
            status = NtQueryInformationFile(hFile, &ioStatusBlock, &positionInfo, sizeof(positionInfo), FilePositionInformation);

            if (!NT_SUCCESS(status)) {
                SetLastError(RtlNtStatusToDosError(status));
                return INVALID_SET_FILE_POINTER;
            }

            positionInfo.CurrentByteOffset.QuadPart += distance.QuadPart;
            break;
        case FILE_END:
            FILE_NETWORK_OPEN_INFORMATION networkInfo;

            status = NtQueryInformationFile(hFile, &ioStatusBlock, &networkInfo, sizeof(networkInfo), FileNetworkOpenInformation);

            if (!NT_SUCCESS(status)) {
                SetLastError(RtlNtStatusToDosError(status));
                return INVALID_SET_FILE_POINTER;
            }

            positionInfo.CurrentByteOffset.QuadPart = networkInfo.EndOfFile.QuadPart + distance.QuadPart;
            break;
    }

    // check for negative file pointer position
    if (positionInfo.CurrentByteOffset.QuadPart < 0) {
        SetLastError(ERROR_NEGATIVE_SEEK);
        return INVALID_SET_FILE_POINTER;
    }

    // we're limited to 32-bit if lpDistanceToMoveHigh is not present
    if (!lpDistanceToMoveHigh && positionInfo.CurrentByteOffset.HighPart != 0) {
        // docs aren't clear which error has to be set in this case, so we just do what ReactOS does
        SetLastError(ERROR_INVALID_PARAMETER);
        return INVALID_SET_FILE_POINTER;
    }

    status = NtSetInformationFile(hFile, &ioStatusBlock, &positionInfo, sizeof(positionInfo), FilePositionInformation);

    if (NT_SUCCESS(status)) {
        if (lpDistanceToMoveHigh) {
            *lpDistanceToMoveHigh = positionInfo.CurrentByteOffset.HighPart;
        }
        return positionInfo.CurrentByteOffset.LowPart;
    } else {
        SetLastError(RtlNtStatusToDosError(status));
        return INVALID_SET_FILE_POINTER;
    }
}

BOOL SetFilePointerEx (HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_POSITION_INFORMATION positionInfo;

    switch (dwMoveMethod) {
        default:
            assert(false);
        case FILE_BEGIN:
            positionInfo.CurrentByteOffset = liDistanceToMove;
            break;
        case FILE_CURRENT:
            status = NtQueryInformationFile(hFile, &ioStatusBlock, &positionInfo, sizeof(positionInfo), FilePositionInformation);

            if(!NT_SUCCESS(status)) {
                SetLastError(RtlNtStatusToDosError(status));
                return FALSE;
            }

            positionInfo.CurrentByteOffset.QuadPart += liDistanceToMove.QuadPart;
            break;
        case FILE_END:
            FILE_NETWORK_OPEN_INFORMATION networkInfo;
            status = NtQueryInformationFile(hFile, &ioStatusBlock, &networkInfo, sizeof(networkInfo), FileNetworkOpenInformation);

            if (!NT_SUCCESS(status)) {
                SetLastError(RtlNtStatusToDosError(status));
                return FALSE;
            }

            positionInfo.CurrentByteOffset.QuadPart = networkInfo.EndOfFile.QuadPart + liDistanceToMove.QuadPart;
            break;
    }

    // check for negative file pointer position
    if (positionInfo.CurrentByteOffset.QuadPart < 0) {
        SetLastError(ERROR_NEGATIVE_SEEK);
        return FALSE;
    }

    status = NtSetInformationFile(hFile, &ioStatusBlock, &positionInfo, sizeof(positionInfo), FilePositionInformation);

    if (NT_SUCCESS(status)) {
        if (lpNewFilePointer) {
            *lpNewFilePointer = positionInfo.CurrentByteOffset;
        }
        return TRUE;
    } else {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }
}

DWORD GetFileSize (HANDLE hFile, LPDWORD lpFileSizeHigh)
{
    LARGE_INTEGER fileSize;

    if (GetFileSizeEx(hFile, &fileSize)) {
        if (lpFileSizeHigh) {
            *lpFileSizeHigh = fileSize.HighPart;
        }
        assert(lpFileSizeHigh || (fileSize.HighPart == 0));
        return fileSize.LowPart;
    } else {
        return INVALID_FILE_SIZE;
    }
}

BOOL GetFileSizeEx (HANDLE hFile, PLARGE_INTEGER lpFileSize)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;
    FILE_NETWORK_OPEN_INFORMATION openInfo;

    assert(lpFileSize != NULL);

    status = NtQueryInformationFile(hFile, &ioStatusBlock, &openInfo, sizeof(openInfo), FileNetworkOpenInformation);
    if (NT_SUCCESS(status)) {
        *lpFileSize = openInfo.EndOfFile;
        return TRUE;
    } else {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }
}
