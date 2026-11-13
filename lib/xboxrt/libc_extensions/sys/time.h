// SPDX-License-Identifier: MIT

#pragma once

/**
 * @file lib/xboxrt/libc_extensions/sys/time.h
 * @brief Declare wall-clock time with microsecond units, shared with lwIP.
 */

#include <time.h>

#ifndef LWIP_TIMEVAL_PRIVATE
#define LWIP_TIMEVAL_PRIVATE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LWIP_HDR_SOCKETS_H
/**
 * @brief Unix-epoch timestamp with a normalized fractional part.
 */
struct timeval
{
    time_t tv_sec; ///< Seconds since 1970-01-01 UTC.
    long tv_usec;  ///< Microseconds in the range 0 through 999999.
};
#endif

/**
 * @brief Read the Xbox wall clock without discarding subsecond precision.
 *
 * @param timeValue Destination timestamp.
 * @param timeZone Obsolete timezone argument, ignored.
 * @return Zero on success, or -1 with errno set to EINVAL for a null destination
 * or EOVERFLOW when the timestamp does not fit nxdk's time_t.
 */
int gettimeofday (struct timeval *timeValue, void *timeZone);

#ifdef __cplusplus
}
#endif
