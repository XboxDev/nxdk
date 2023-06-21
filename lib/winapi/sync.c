// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2020 Stefan Schmidt
// SPDX-FileCopyrightText: 2020 Jannik Vogel
// SPDX-FileCopyrightText: 2022 Ryan Wendland

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

#define SRW_GLOBAL_LOCK_MASK ((uintptr_t)0x40000000)
#define SRW_READER_LOCK_MASK ((uintptr_t)0x80000000)
#define SRW_READER_COUNT_MASK ~(SRW_GLOBAL_LOCK_MASK | SRW_READER_LOCK_MASK)

static bool TryAcquireLockMask (uintptr_t *lock, uintptr_t mask)
{
    uintptr_t unlocked_val = *lock & ~mask;
    return __sync_bool_compare_and_swap(lock, unlocked_val, unlocked_val | mask);
}

static void ReleaseLockMask (uintptr_t *lock, uintptr_t mask)
{
    __sync_fetch_and_and(lock, ~mask);
}

void AcquireSRWLockExclusive (PSRWLOCK SRWLock)
{
    // Grab the global lock (second-highest bit)
    while (!TryAcquireLockMask(&SRWLock->Ptr, SRW_GLOBAL_LOCK_MASK)) {
        SwitchToThread();
    }
}

void AcquireSRWLockShared (PSRWLOCK SRWLock)
{
    // Grab the reader lock
    while (!TryAcquireLockMask(&SRWLock->Ptr, SRW_READER_LOCK_MASK)) {
        SwitchToThread();
    }

    // Increase reader count by one
    uintptr_t prev_val = __atomic_fetch_add(&SRWLock->Ptr, 1, __ATOMIC_ACQ_REL);
    if ((prev_val & SRW_READER_COUNT_MASK) == 0) {
        // If we're the first reader, grab the global lock, too
        AcquireSRWLockExclusive(SRWLock);
    }

    // Release the reader lock
    ReleaseLockMask(&SRWLock->Ptr, SRW_READER_LOCK_MASK);
}

void InitializeSRWLock (PSRWLOCK SRWLock)
{
    __atomic_store_n(&SRWLock->Ptr, 0, __ATOMIC_RELEASE);
}

void ReleaseSRWLockExclusive (PSRWLOCK SRWLock)
{
    // Release the global lock (second-highest bit)
    ReleaseLockMask(&SRWLock->Ptr, SRW_GLOBAL_LOCK_MASK);
}

void ReleaseSRWLockShared (PSRWLOCK SRWLock)
{
    // Grab the reader lock
    while (!TryAcquireLockMask(&SRWLock->Ptr, SRW_READER_LOCK_MASK)) {
        SwitchToThread();
    }

    // Decrease reader count by one
    uintptr_t prev_val = __atomic_fetch_sub(&SRWLock->Ptr, 1, __ATOMIC_ACQ_REL);
    assert((prev_val & SRW_READER_COUNT_MASK) != 0);
    if ((prev_val & SRW_READER_COUNT_MASK) == 1) {
        // If we're the last reader, release the global lock
        ReleaseSRWLockExclusive(SRWLock);
    }

    // Release the reader lock
    ReleaseLockMask(&SRWLock->Ptr, SRW_READER_LOCK_MASK);
}

BOOLEAN TryAcquireSRWLockExclusive (PSRWLOCK SRWLock)
{
    // Try once to grab the global lock, return whether we succeeded
    return TryAcquireLockMask(&SRWLock->Ptr, SRW_GLOBAL_LOCK_MASK);
}

BOOLEAN TryAcquireSRWLockShared (PSRWLOCK SRWLock)
{
    bool success;

    // Grab the reader lock
    while (!TryAcquireLockMask(&SRWLock->Ptr, SRW_READER_LOCK_MASK)) {
        SwitchToThread();
    }

    // Increase reader count by one
    uintptr_t prev_val = __atomic_fetch_add(&SRWLock->Ptr, 1, __ATOMIC_ACQ_REL);
    if ((prev_val & SRW_READER_COUNT_MASK) == 0) {
        // If we're the first reader, try grab the global lock
        success = TryAcquireSRWLockExclusive(SRWLock);
    } else {
        success = TRUE;
    }

    if (!success) {
        // If we didn't succeed, we decrement the reader count again
        __atomic_fetch_sub(&SRWLock->Ptr, 1, __ATOMIC_ACQ_REL);
    }

    // Release the reader lock
    ReleaseLockMask(&SRWLock->Ptr, SRW_READER_LOCK_MASK);

    return success;
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

static BOOL WINAPI condvar_init (PINIT_ONCE InitOnce, PVOID Parameter, PVOID *Context)
{
    PCONDITION_VARIABLE ConditionVariable = Parameter;
    NTSTATUS status;
    assert(ConditionVariable->waitCount == 0);
    assert(ConditionVariable->eventHandles[0] == INVALID_HANDLE_VALUE);
    assert(ConditionVariable->eventHandles[1] == INVALID_HANDLE_VALUE);
    status = NtCreateEvent(&ConditionVariable->eventHandles[0], NULL, SynchronizationEvent, FALSE);
    assert(NT_SUCCESS(status));
    status = NtCreateEvent(&ConditionVariable->eventHandles[1], NULL, NotificationEvent, FALSE);
    assert(NT_SUCCESS(status));
    return true;
}

VOID InitializeConditionVariable (PCONDITION_VARIABLE ConditionVariable)
{
    ConditionVariable->initOnce.Ptr = 0;
    __atomic_store_n(&ConditionVariable->waitCount, 0, __ATOMIC_RELEASE);
    ConditionVariable->eventHandles[0] = INVALID_HANDLE_VALUE;
    ConditionVariable->eventHandles[1] = INVALID_HANDLE_VALUE;

    BOOL success = InitOnceExecuteOnce(&ConditionVariable->initOnce, condvar_init, ConditionVariable, NULL);
    assert(success);
}

BOOL SleepConditionVariableCS (PCONDITION_VARIABLE ConditionVariable, PCRITICAL_SECTION CriticalSection, DWORD dwMilliseconds)
{
    BOOL success = InitOnceExecuteOnce(&ConditionVariable->initOnce, condvar_init, ConditionVariable, NULL);
    assert(success);

    NTSTATUS status;
    LARGE_INTEGER waitTime;
    LARGE_INTEGER *waitTimePtr = NULL;

    if (dwMilliseconds != INFINITE) {
        waitTime.QuadPart = dwMilliseconds * 1000;
        waitTimePtr = &waitTime;
    }

    LeaveCriticalSection(CriticalSection);
    __atomic_add_fetch(&ConditionVariable->waitCount, 1, __ATOMIC_ACQ_REL);
    status = NtWaitForMultipleObjectsEx(2, ConditionVariable->eventHandles, WaitAny, UserMode, FALSE, waitTimePtr);
    if (status == WAIT_FAILED || status == STATUS_TIMEOUT) {
        __atomic_sub_fetch(&ConditionVariable->waitCount, 1, __ATOMIC_ACQ_REL);
        EnterCriticalSection(CriticalSection);
        return FALSE;
    }

    int oldCount = __atomic_fetch_sub(&ConditionVariable->waitCount, 1, __ATOMIC_ACQ_REL);
    if (oldCount == 1) {
        // Last waiter needs to manually reset the broadcast-object
        status = NtClearEvent(ConditionVariable->eventHandles[1]);
        if (!NT_SUCCESS(status)) {
            EnterCriticalSection(CriticalSection);
            return FALSE;
        }
    }

    EnterCriticalSection(CriticalSection);
    return TRUE;
}

BOOL SleepConditionVariableSRW (PCONDITION_VARIABLE ConditionVariable, PSRWLOCK SRWLock, DWORD dwMilliseconds, ULONG Flags)
{
    BOOL success = InitOnceExecuteOnce(&ConditionVariable->initOnce, condvar_init, ConditionVariable, NULL);
    assert(success);

    NTSTATUS status;
    LARGE_INTEGER waitTime;
    LARGE_INTEGER *waitTimePtr = NULL;

    if (dwMilliseconds != INFINITE) {
        waitTime.QuadPart = dwMilliseconds * 1000;
        waitTimePtr = &waitTime;
    }

    if (Flags == CONDITION_VARIABLE_LOCKMODE_SHARED) {
        ReleaseSRWLockShared(SRWLock);
    } else {
        ReleaseSRWLockExclusive(SRWLock);
    }
    __atomic_add_fetch(&ConditionVariable->waitCount, 1, __ATOMIC_ACQ_REL);
    status = NtWaitForMultipleObjectsEx(2, ConditionVariable->eventHandles, WaitAny, UserMode, FALSE, waitTimePtr);
    if (status == WAIT_FAILED || status == STATUS_TIMEOUT) {
        __atomic_sub_fetch(&ConditionVariable->waitCount, 1, __ATOMIC_ACQ_REL);
        if (Flags == CONDITION_VARIABLE_LOCKMODE_SHARED) {
            AcquireSRWLockShared(SRWLock);
        } else {
            AcquireSRWLockExclusive(SRWLock);
        }
        return FALSE;
    }

    int oldCount = __atomic_fetch_sub(&ConditionVariable->waitCount, 1, __ATOMIC_ACQ_REL);
    if (oldCount == 1) {
        // Last waiter needs to manually reset the broadcast-object
        status = NtClearEvent(ConditionVariable->eventHandles[1]);
        if (!NT_SUCCESS(status)) {
            if (Flags == CONDITION_VARIABLE_LOCKMODE_SHARED) {
                AcquireSRWLockShared(SRWLock);
            } else {
                AcquireSRWLockExclusive(SRWLock);
            }
            return FALSE;
        }
    }

    if (Flags == CONDITION_VARIABLE_LOCKMODE_SHARED) {
        AcquireSRWLockShared(SRWLock);
    } else {
        AcquireSRWLockExclusive(SRWLock);
    }
    return TRUE;
}

VOID WakeConditionVariable (PCONDITION_VARIABLE ConditionVariable)
{
    BOOL success = InitOnceExecuteOnce(&ConditionVariable->initOnce, condvar_init, ConditionVariable, NULL);
    assert(success);

    NTSTATUS status;
    status = NtSetEvent(ConditionVariable->eventHandles[0], NULL);
    assert(NT_SUCCESS(status));
}

VOID WakeAllConditionVariable (PCONDITION_VARIABLE ConditionVariable)
{
    BOOL success = InitOnceExecuteOnce(&ConditionVariable->initOnce, condvar_init, ConditionVariable, NULL);
    assert(success);

    NTSTATUS status;
    status = NtSetEvent(ConditionVariable->eventHandles[1], NULL);
    assert(NT_SUCCESS(status));
}

VOID UninitializeConditionVariable (PCONDITION_VARIABLE ConditionVariable)
{
    ConditionVariable->initOnce.Ptr = (PVOID)INITONCE_DONE;
    NtClose(ConditionVariable->eventHandles[0]);
    NtClose(ConditionVariable->eventHandles[1]);
    ConditionVariable->eventHandles[0] = INVALID_HANDLE_VALUE;
    ConditionVariable->eventHandles[1] = INVALID_HANDLE_VALUE;
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

HANDLE CreateMutexA (LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName)
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

    status = NtCreateMutant(&handle, obj_attributes_ptr, bInitialOwner);
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

BOOL ReleaseMutex (HANDLE hMutex)
{
    NTSTATUS status;

    status = NtReleaseMutant(hMutex, NULL);
    if (NT_SUCCESS(status)) {
        return TRUE;
    }

    SetLastError(RtlNtStatusToDosError(status));
    return FALSE;
}

HANDLE CreateEventA (LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName)
{
    NTSTATUS status;
    HANDLE handle;
    ANSI_STRING obj_name;
    OBJECT_ATTRIBUTES obj_attributes;
    POBJECT_ATTRIBUTES obj_attributes_ptr;
    EVENT_TYPE event;

    if (lpName) {
        RtlInitAnsiString(&obj_name, lpName);
        InitializeObjectAttributes(&obj_attributes, &obj_name, OBJ_OPENIF, ObWin32NamedObjectsDirectory(), NULL);
        obj_attributes_ptr = &obj_attributes;
    } else {
        obj_attributes_ptr = NULL;
    }

    if (bManualReset) {
        event = NotificationEvent;
    } else {
        event = SynchronizationEvent;
    }

    status = NtCreateEvent(&handle, obj_attributes_ptr, event, bInitialState);
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

BOOL SetEvent (HANDLE hEvent)
{
    NTSTATUS status;

    status = NtSetEvent(hEvent, NULL);
    if (NT_SUCCESS(status)) {
        return TRUE;
    }

    SetLastError(RtlNtStatusToDosError(status));
    return FALSE;
}

BOOL ResetEvent (HANDLE hEvent)
{
    NTSTATUS status;

    status = NtClearEvent(hEvent);
    if (NT_SUCCESS(status)) {
        return TRUE;
    }

    SetLastError(RtlNtStatusToDosError(status));
    return FALSE;
}

BOOL PulseEvent (HANDLE hEvent)
{
    NTSTATUS status;

    status = NtPulseEvent(hEvent, NULL);
    if (NT_SUCCESS(status)) {
        return TRUE;
    }

    SetLastError(RtlNtStatusToDosError(status));
    return FALSE;
}
