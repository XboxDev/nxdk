#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

int _cnd_wait (cnd_t *cond, mtx_t *mtx, LARGE_INTEGER *timepoint)
{
    NTSTATUS status;

    mtx_unlock(mtx);
    atomic_fetch_add(&cond->waitCount, 1);
    status = NtWaitForMultipleObjectsEx(2, cond->eventHandles, WaitAny, UserMode, FALSE, timepoint);
    if (status == WAIT_FAILED)
    {
        atomic_fetch_sub(&cond->waitCount, 1);
        mtx_lock(mtx);
        return thrd_error;
    }
    else if (status == STATUS_TIMEOUT)
    {
        atomic_fetch_sub(&cond->waitCount, 1);
        mtx_lock(mtx);
        return thrd_timedout;
    }

    int oldCount = atomic_fetch_sub(&cond->waitCount, 1);
    if (oldCount == 1) {
        // Last waiter needs to manually reset the broadcast-object
        status = NtClearEvent(cond->eventHandles[1]);
        if (!NT_SUCCESS(status)) {
            mtx_lock(mtx);
            return thrd_error;
        }
    }

    mtx_lock(mtx);
    return thrd_success;
}

int cnd_wait (cnd_t *cond, mtx_t *mtx)
{
    return _cnd_wait(cond, mtx, NULL);
}

int cnd_timedwait (cnd_t *restrict cond, mtx_t *restrict mtx, const struct timespec *restrict ts)
{
    LARGE_INTEGER timepoint;
    timepoint.QuadPart = ((long long)ts->tv_sec)*10000000 + ((long long)ts->tv_nsec)/100;
    if (ts->tv_nsec % 100 != 0) timepoint.QuadPart++;

    return _cnd_wait(cond, mtx, &timepoint);
}
