#include <processthreadsapi.h>
#include <xboxkrnl/xboxkrnl.h>

BOOL SwitchToThread (VOID)
{
    if (NtYieldExecution() != STATUS_NO_YIELD_PERFORMED) {
        return TRUE;
    } else {
        return FALSE;
    }
}
