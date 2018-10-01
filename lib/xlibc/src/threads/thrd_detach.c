#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

int thrd_detach (thrd_t thr)
{
    if (thr.handle == NULL)
    {
        return thrd_error;
    }

    if (NT_SUCCESS(NtClose(thr.handle)))
    {
        return thrd_success;
    }
    else
    {
        return thrd_error;
    }
}
