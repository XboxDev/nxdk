#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

int mtx_trylock (mtx_t *mtx)
{
    NTSTATUS status;
    LARGE_INTEGER interval;
    interval.QuadPart = 0;

    status = NtWaitForSingleObject(mtx->handle, FALSE, &interval);
    if (status == STATUS_WAIT_0)
    {
        return thrd_success;
    }

    return thrd_busy;
}
