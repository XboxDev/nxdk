#include <time.h>
#include "xboxkrnl/xboxkrnl.h"

int timespec_get (struct timespec *ts, int base)
{
    LARGE_INTEGER currentTime;

    if (base != TIME_UTC) return 0;

    // Get the current time in GMT, with 100ns units
    KeQuerySystemTime(&currentTime);

    // Convert from 100ns units into sec+ns
    ts->tv_sec = currentTime.QuadPart / 10000000;
    ts->tv_nsec = (currentTime.QuadPart % 10000000) * 100;

    return base;
}
