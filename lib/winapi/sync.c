#include <synchapi.h>
#include <stdbool.h>
#include <xboxkrnl/xboxkrnl.h>

VOID InitializeCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
{
    RtlInitializeCriticalSection(lpCriticalSection);
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
