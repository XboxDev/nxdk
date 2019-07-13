#include <fileapi.h>
#include <hal/winerror.h>
#include <assert.h>
#include <stdbool.h>
#include <xboxkrnl/xboxkrnl.h>

BOOL ReadFile (HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    NTSTATUS status;
    IO_STATUS_BLOCK ioStatusBlock;

    // overlapped I/O not supported yet
    assert(!lpOverlapped);

    // A null pointer makes the code crash on Windows, we'd rather catch it
    assert(lpNumberOfBytesRead);
    *lpNumberOfBytesRead = 0;

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

    // overlapped I/O not supported yet
    assert(!lpOverlapped);

    // A null pointer makes the code crash on Windows, we'd rather catch it
    assert(lpNumberOfBytesWritten);
    *lpNumberOfBytesWritten = 0;

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
