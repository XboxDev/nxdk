#include <processthreadsapi.h>
#include <winbase.h>
#include <xboxkrnl/xboxkrnl.h>

HANDLE GetCurrentThread (VOID)
{
    return (HANDLE)-2;
}

DWORD GetCurrentThreadId (VOID)
{
    return (DWORD)((PETHREAD)KeGetCurrentThread())->UniqueThread;
}

DWORD GetThreadId (HANDLE Thread)
{
    PETHREAD threadObject;
    HANDLE id;
    NTSTATUS status;

    status = ObReferenceObjectByHandle(Thread, NULL, (PVOID)&threadObject);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return 0;
    }

    id = threadObject->UniqueThread;
    ObfDereferenceObject(threadObject);

    return (DWORD)id;
}

BOOL SwitchToThread (VOID)
{
    if (NtYieldExecution() != STATUS_NO_YIELD_PERFORMED) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL SetThreadPriority (HANDLE hThread, int nPriority)
{
    NTSTATUS status;
    PETHREAD thread;

    status = ObReferenceObjectByHandle(hThread, &PsThreadObjectType, (PVOID)&thread);
    if (!NT_SUCCESS(status)) {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }

    LONG priority = (LONG)nPriority;
    if (priority == THREAD_PRIORITY_TIME_CRITICAL) {
        priority = (HIGH_PRIORITY + 1) / 2;
    } else if (priority == THREAD_PRIORITY_IDLE) {
        priority = -((HIGH_PRIORITY + 1) / 2);
    }

    KeSetBasePriorityThread(&thread->Tcb, priority);

    ObfDereferenceObject(thread);
    return TRUE;
}
