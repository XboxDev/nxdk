#include <synchapi.h>
#include <assert.h>
#include <stdbool.h>
#include <processthreadsapi.h>
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

#define INITONCE_MASK (((uintptr_t)1 << INIT_ONCE_CTX_RESERVED_BITS) - 1)
#define INITONCE_UNINITIALIZED 0
#define INITONCE_IN_PROGRESS 1
#define INITONCE_ASYNC_IN_PROGRESS 2
#define INITONCE_DONE 3

BOOL InitOnceBeginInitialize (LPINIT_ONCE lpInitOnce, DWORD dwFlags, PBOOL fPending, LPVOID *lpContext)
{
    assert(fPending != NULL);

    if (dwFlags & INIT_ONCE_CHECK_ONLY) {
        if (dwFlags & INIT_ONCE_ASYNC) {
            *fPending = FALSE;
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;
        }
        if (((ULONG_PTR)lpInitOnce->Ptr & INITONCE_MASK) != INITONCE_DONE) {
            *fPending = TRUE;
            SetLastError(ERROR_GEN_FAILURE);
            return FALSE;
        }

        if (lpContext) {
            *lpContext = (LPVOID)((ULONG_PTR)lpInitOnce->Ptr & ~INITONCE_MASK);
        }
        // success
        *fPending = FALSE;
        return TRUE;
    }

    while (true) {
        switch ((ULONG_PTR)__atomic_load_n(&lpInitOnce->Ptr, __ATOMIC_ACQUIRE) & INITONCE_MASK) {
            case INITONCE_UNINITIALIZED:
                if (__sync_bool_compare_and_swap(&lpInitOnce->Ptr, (PVOID)INITONCE_UNINITIALIZED, (dwFlags & INIT_ONCE_ASYNC) ? (PVOID)INITONCE_ASYNC_IN_PROGRESS : (PVOID)INITONCE_IN_PROGRESS)) {
                    *fPending = TRUE;
                    return TRUE;
                }
                break;
            case INITONCE_IN_PROGRESS:
                if (dwFlags & INIT_ONCE_ASYNC) {
                    SetLastError(ERROR_INVALID_PARAMETER);
                    *fPending = TRUE;
                    return FALSE;
                }
                SwitchToThread();
                break;
            case INITONCE_ASYNC_IN_PROGRESS:
                if (!(dwFlags & INIT_ONCE_ASYNC)) {
                    SetLastError(ERROR_INVALID_PARAMETER);
                    *fPending = TRUE;
                    return FALSE;
                }
                *fPending = TRUE;
                return TRUE;
            case INITONCE_DONE:
                if (lpContext) {
                    *lpContext = (PVOID)((ULONG_PTR)lpInitOnce->Ptr & ~INITONCE_MASK);
                }
                *fPending = FALSE;
                return TRUE;
        }
    }
}

BOOL InitOnceExecuteOnce (PINIT_ONCE InitOnce, PINIT_ONCE_FN InitFn, PVOID Parameter, LPVOID *Context)
{
    if ((((ULONG_PTR)Context) & INITONCE_MASK) != INITONCE_UNINITIALIZED) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    BOOL fPending;
    BOOL ret;
    ret = InitOnceBeginInitialize(InitOnce, 0, &fPending, Context);
    if (!fPending) {
        return ret;
    }

    if (InitFn(InitOnce, Parameter, Context)) {
        return InitOnceComplete(InitOnce, 0, Context ? *Context : NULL);
    }

    BOOL success = InitOnceComplete(InitOnce, INIT_ONCE_INIT_FAILED, NULL);
    assert(success);
    return FALSE;
}

BOOL InitOnceComplete (LPINIT_ONCE lpInitOnce, DWORD dwFlags, LPVOID lpContext)
{
    if (((ULONG_PTR)lpContext & INITONCE_MASK) != INITONCE_UNINITIALIZED) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (dwFlags & INIT_ONCE_INIT_FAILED) {
        if (lpContext || (dwFlags & INIT_ONCE_ASYNC)) {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;
        }
    } else {
        lpContext = (PVOID)((ULONG_PTR)lpContext | INITONCE_DONE);
    }

    while (true) {
        PVOID cur_val = lpInitOnce->Ptr;

        switch ((ULONG_PTR)lpInitOnce->Ptr & INITONCE_MASK) {
            case INITONCE_IN_PROGRESS:
                if (__sync_val_compare_and_swap(&lpInitOnce->Ptr, cur_val, lpContext) != cur_val) {
                    break; // failed to swap the value, try again
                }

                // init done, waiters continue because they were spinning in InitOnceBeginInitialize
                return TRUE;
            case INITONCE_ASYNC_IN_PROGRESS:
                if(!(dwFlags & INIT_ONCE_ASYNC)) {
                    SetLastError(ERROR_INVALID_PARAMETER);
                    return FALSE;
                }

                if (__sync_val_compare_and_swap(&lpInitOnce->Ptr, cur_val, lpContext) == cur_val) {
                    // async init done
                    return TRUE;
                }
                break;
            default:
                SetLastError(ERROR_GEN_FAILURE);
                return FALSE;
        }
    }
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

DWORD WaitForSingleObject (HANDLE hHandle, DWORD dwMilliseconds)
{
    return WaitForSingleObjectEx(hHandle, dwMilliseconds, FALSE);
}

DWORD WaitForMultipleObjectsEx (DWORD nCount, const HANDLE *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds, BOOL bAlertable)
{
    LARGE_INTEGER duration;
    duration.QuadPart = ((LONGLONG)dwMilliseconds) * -10000;

    while (true) {
        NTSTATUS status = NtWaitForMultipleObjectsEx(nCount, lpHandles, bWaitAll ? WaitAll : WaitAny, UserMode, bAlertable, &duration);

        if (status == STATUS_ALERTED) continue;

        if (!NT_SUCCESS(status)) {
            SetLastError(RtlNtStatusToDosError(status));
            return WAIT_FAILED;
        }

        return status;
    }
}

DWORD WaitForMultipleObjects (DWORD nCount, const HANDLE *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds)
{
    return WaitForMultipleObjectsEx(nCount, lpHandles, bWaitAll, dwMilliseconds, FALSE);
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
