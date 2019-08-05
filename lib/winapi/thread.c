#include <processthreadsapi.h>
#include <winbase.h>
#include <xboxkrnl/xboxkrnl.h>

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
