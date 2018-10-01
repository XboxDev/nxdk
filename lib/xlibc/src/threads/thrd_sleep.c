#include <threads.h>
#include "xboxkrnl/xboxkrnl.h"

int thrd_sleep (const struct timespec *duration, struct timespec *remaining)
{
    LARGE_INTEGER interval; // Units of 100 nanoseconds
    NTSTATUS status;
    DWORD oldTickCount;

    oldTickCount = KeTickCount;

    // Convert the s+ns interval into 100ns units (rounding up!)
    interval.QuadPart = ((long long)duration->tv_sec)*10000000 + ((long long)duration->tv_nsec)/100;
    if (duration->tv_nsec % 100 != 0) interval.QuadPart++;
    // Negative values indicate relative time
    interval.QuadPart = -interval.QuadPart;
    status = KeDelayExecutionThread(UserMode, FALSE, &interval);

    if (NT_SUCCESS(status))
    {
        if (remaining)
        {
            remaining->tv_sec = 0;
            remaining->tv_nsec = 0;
        }
        return 0;
    }

    // If we were interrupted, we may need to calculate the remaining time
    if (status == STATUS_USER_APC)
    {
        if (remaining)
        {
            // Precision degrades to tick-resolution here (10-16ms)
            DWORD ticksSlept = KeTickCount - oldTickCount;
            // Calculate remaining time (in 100ns units), converting the ticks (1 tick = 1 ms)
            long long timeUnitsRemaining = interval.QuadPart - (ticksSlept*10000);

            remaining->tv_sec = timeUnitsRemaining / 10000000;
            remaining->tv_nsec = (timeUnitsRemaining % 10000000) * 100;
        }
        return -1;
    }

    return -2;
}
