#include <threads.h>
#include <time.h>
#include "xboxkrnl/xboxkrnl.h"

int mtx_timedlock (mtx_t *restrict mtx, const struct timespec *restrict ts)
{
    NTSTATUS status;
    LARGE_INTEGER interval;
    struct timespec currentTime;

    timespec_get(&currentTime, TIME_UTC);

    if ((currentTime.tv_sec > ts->tv_sec) || ((currentTime.tv_sec == ts->tv_sec) && (currentTime.tv_nsec >= ts->tv_nsec)))
    {
        interval.QuadPart = 0;
    }
    else
    {
        // Convert to 100ns resolution
        long long current = ((long long)currentTime.tv_sec)*10000000 + currentTime.tv_nsec/100;
        if (currentTime.tv_nsec % 100 != 0) current++;

        long long until = ((long long)ts->tv_sec)*10000000 + ts->tv_nsec/100;
        if (ts->tv_nsec % 100 != 0) until++;

        interval.QuadPart = until-current;
        interval.QuadPart = -interval.QuadPart;
    }

    status = NtWaitForSingleObject(mtx->handle, FALSE, &interval);
    if (status == STATUS_WAIT_0)
    {
        return thrd_success;
    }
    else if (status == STATUS_TIMEOUT)
    {
        return thrd_timedout;
    }

    return thrd_error;
}
