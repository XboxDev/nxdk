#include <synchapi.h>
#include <stdbool.h>
#include <winbase.h>
#include <winerror.h>
#include <xboxkrnl/xboxkrnl.h>

VOID InitializeCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
{
    RtlInitializeCriticalSection(lpCriticalSection);
}

BOOL InitializeCriticalSectionAndSpinCount (LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount)
{
    // From the MSDN: The spin count for the critical section object
    // On single-processor systems, the spin count is ignored and the critical
    // section spin count is set to 0 (zero).
    InitializeCriticalSection(lpCriticalSection);
    return TRUE;
}

VOID DeleteCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
{
    RtlDeleteCriticalSection(lpCriticalSection);
}

VOID EnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
{
    RtlEnterCriticalSection(lpCriticalSection);
}

BOOL TryEnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
{
    return RtlTryEnterCriticalSection(lpCriticalSection);
}

VOID LeaveCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
{
    RtlLeaveCriticalSection(lpCriticalSection);
}

VOID Sleep (DWORD dwMilliseconds)
{
    SleepEx(dwMilliseconds, FALSE);
}

DWORD SleepEx (DWORD dwMilliseconds, BOOL bAlertable)
{
    LARGE_INTEGER duration;
    duration.QuadPart = ((LONGLONG)dwMilliseconds) * -10000;
    while (true) {
        NTSTATUS status = KeDelayExecutionThread(UserMode, bAlertable, &duration);
        if (status != STATUS_ALERTED) {
            return status == STATUS_USER_APC ? WAIT_IO_COMPLETION : 0;
        }
    }
}

DWORD WaitForSingleObjectEx (HANDLE hHandle, DWORD dwMilliseconds, BOOL bAlertable)
{
    LARGE_INTEGER duration;
    duration.QuadPart = ((LONGLONG)dwMilliseconds) * -10000;

    while (true) {
        NTSTATUS status = NtWaitForSingleObjectEx(hHandle, UserMode, bAlertable, &duration);

        if (status == STATUS_ALERTED) continue;

        if (!NT_SUCCESS(status)) {
            SetLastError(RtlNtStatusToDosError(status));
            return WAIT_FAILED;
        }

        return status;
    }
}

HANDLE CreateSemaphore (LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING obj_name;
    OBJECT_ATTRIBUTES obj_attributes;
    POBJECT_ATTRIBUTES obj_attributes_ptr;

    if (lpName) {
        RtlInitAnsiString(&obj_name, lpName);
        InitializeObjectAttributes(&obj_attributes, &obj_name, OBJ_OPENIF, ObWin32NamedObjectsDirectory(), NULL);
        obj_attributes_ptr = &obj_attributes;
    } else {
        obj_attributes_ptr = NULL;
    }

    status = NtCreateSemaphore(&handle, obj_attributes_ptr, lInitialCount, lMaximumCount);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return NULL;
    }

    if (status == STATUS_OBJECT_NAME_EXISTS) {
        SetLastError(ERROR_ALREADY_EXISTS);
    } else {
        SetLastError(0);
    }

    return handle;
}

BOOL ReleaseSemaphore (HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount)
{
    NTSTATUS status;

    status = NtReleaseSemaphore(hSemaphore, lReleaseCount, lpPreviousCount);
    if (NT_SUCCESS(status)) {
        return TRUE;
    }

    SetLastError(RtlNtStatusToDosError(status));
    return FALSE;
}
