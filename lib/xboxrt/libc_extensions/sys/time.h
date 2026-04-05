// SPDX-License-Identifier: MIT

#pragma once

/*
 * Provide a standalone POSIX-style sys/time.h for nxdk builds.
 *
 * lwIP can define timeval from its socket headers, but many third-party
 * libraries include sys/time.h directly before they include any socket header.
 * Setting LWIP_TIMEVAL_PRIVATE to 0 keeps later lwIP includes from defining a
 * second timeval.
 */

#include <time.h>

#ifndef LWIP_TIMEVAL_PRIVATE
#define LWIP_TIMEVAL_PRIVATE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

#ifndef LWIP_HDR_SOCKETS_H
struct timeval {
    time_t tv_sec;
    long tv_usec;
};
#endif

static inline int gettimeofday(struct timeval *timeValue, struct timezone *timeZone)
{
    if (timeValue != NULL) {
        timeValue->tv_sec = time(NULL);
        timeValue->tv_usec = 0;
    }

    if (timeZone != NULL) {
        timeZone->tz_minuteswest = 0;
        timeZone->tz_dsttime = 0;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
