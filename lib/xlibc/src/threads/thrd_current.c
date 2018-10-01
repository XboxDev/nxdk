#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

thrd_t thrd_current (void)
{
    thrd_t thrd;
    thrd.handle = NULL;
    thrd.id = ((PETHREAD)KeGetCurrentThread())->UniqueThread;
    return thrd;
}
