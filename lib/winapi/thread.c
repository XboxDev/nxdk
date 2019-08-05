#include <processthreadsapi.h>
#include <xboxkrnl/xboxkrnl.h>

DWORD GetCurrentThreadId (VOID)
{
    return (DWORD)((PETHREAD)KeGetCurrentThread())->UniqueThread;
}

BOOL SwitchToThread (VOID)
{
    if (NtYieldExecution() != STATUS_NO_YIELD_PERFORMED) {
        return TRUE;
    } else {
        return FALSE;
    }
}
