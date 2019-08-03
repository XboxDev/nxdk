#include <synchapi.h>
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
