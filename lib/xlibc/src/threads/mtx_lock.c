#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

int mtx_lock (mtx_t *mtx)
{
    NTSTATUS status = NtWaitForSingleObject(mtx->handle, FALSE, NULL);
    if (status != STATUS_WAIT_0)
    {
        return thrd_success;
    }

    return thrd_error;
}
