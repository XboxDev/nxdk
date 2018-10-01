#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

_Noreturn void thrd_exit (int res)
{
    PsTerminateSystemThread(res);
}
