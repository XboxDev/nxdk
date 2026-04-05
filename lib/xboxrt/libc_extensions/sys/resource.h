// SPDX-License-Identifier: MIT

#pragma once

/*
 * Provide the small sys/resource.h subset required by libraries that probe for
 * process usage statistics during build-time utility code paths.
 */

#include <stddef.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RUSAGE_SELF 0

struct rusage {
    struct timeval ru_utime;
    struct timeval ru_stime;
};

static inline int getrusage(int who, struct rusage *usage)
{
    (void) who;
    if (usage != NULL) {
        usage->ru_utime.tv_sec = 0;
        usage->ru_utime.tv_usec = 0;
        usage->ru_stime.tv_sec = 0;
        usage->ru_stime.tv_usec = 0;
    }
    return -1;
}

#ifdef __cplusplus
}
#endif
