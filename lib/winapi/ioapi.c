// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2023 Ryan Wendland

#include <synchapi.h>
#include <winbase.h>
#include <winerror.h>
#include <xboxkrnl/xboxkrnl.h>

BOOL GetOverlappedResult (HANDLE hFile, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait)
{
    DWORD status;
    HANDLE waitHandle;

    status = (DWORD)lpOverlapped->Internal;

    if (status == STATUS_PENDING && bWait == FALSE) {
        SetLastError(ERROR_IO_INCOMPLETE);
        return FALSE;
    }

    // If the hEvent member of the OVERLAPPED structure is NULL, the system
    // uses the state of the hFile handle to signal when the operation has been completed
    if (lpOverlapped->hEvent == NULL) {
        waitHandle = hFile;
    } else {
        waitHandle = lpOverlapped->hEvent;
    }

    if (status == STATUS_PENDING) {
        if (WaitForSingleObject(waitHandle, INFINITE) != WAIT_OBJECT_0) {
            SetLastError(ERROR_IO_INCOMPLETE);
            return FALSE;
        }
        // Get final status of the transfer
        status = (DWORD)lpOverlapped->Internal;
    }

    // InternalHigh contains the actual number of bytes transferred for the I/O request
    *lpNumberOfBytesTransferred = (DWORD)lpOverlapped->InternalHigh;

    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }
    return TRUE;
}
