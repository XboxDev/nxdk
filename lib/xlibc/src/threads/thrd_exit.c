#include <threads.h>
#include <_tss.h>
#include "xboxkrnl/xboxkrnl.h"

_Noreturn void thrd_exit (int res)
{
    _xlibc_tss_cleanup();
    PsTerminateSystemThread(res);
}
