#include <fileapi.h>
#include <assert.h>
#include <xboxkrnl/xboxkrnl.h>

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
