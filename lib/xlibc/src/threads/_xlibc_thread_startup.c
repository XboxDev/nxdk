#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

VOID NTAPI _xlibc_thread_startup (PKSTART_ROUTINE StartRoutine, PVOID StartContext)
{
    int res;
    res = ((thrd_start_t)StartRoutine)(StartContext);
    thrd_exit(res);
}
