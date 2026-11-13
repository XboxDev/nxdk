// SPDX-License-Identifier: MIT

/**
 * @file lib/xboxrt/libc_extensions/gettimeofday.c
 * @brief Convert Xbox kernel timestamps to Unix wall-clock timestamps.
 */
#include <errno.h>
#include <limits.h>
#include <sys/time.h>
#include <xboxkrnl/xboxkrnl.h>

int gettimeofday (struct timeval *timeValue, void *timeZone)
{
    (void)timeZone;
    if (timeValue == NULL) {
        errno = EINVAL;
        return -1;
    }

    LARGE_INTEGER systemTime;
    KeQuerySystemTime(&systemTime);
    long long seconds = systemTime.QuadPart / 10000000LL - 11644473600LL;
    long long remainder = systemTime.QuadPart % 10000000LL;
    if (remainder < 0) {
        --seconds;
        remainder += 10000000LL;
    }

    // nxdk's time_t is a signed long, as configured by the Xbox PDCLib port.
    if (seconds < LONG_MIN || seconds > LONG_MAX) {
        errno = EOVERFLOW;
        return -1;
    }

    timeValue->tv_sec = (time_t)seconds;
    timeValue->tv_usec = (long)(remainder / 10LL);
    return 0;
}
