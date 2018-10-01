#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

void thrd_yield (void)
{
    NtYieldExecution();
}
