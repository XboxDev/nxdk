// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#include <profileapi.h>
#ifdef USE_RDTSC_FOR_FREQ
#include <synchapi.h>
#endif
#include <assert.h>
#include <stddef.h>
#include <xboxkrnl/xboxkrnl.h>

#ifdef USE_RDTSC_FOR_FREQ
static LARGE_INTEGER frequency = {0, 0};
static void __attribute__((constructor)) PrimeQueryPerformanceFrequency ()
{
    #define AVG_SET 2
    ULARGE_INTEGER f_rdtsc, avg = {0, 0}, s_rdtsc;
    ULONG f_ticks = 0, s_ticks = 0;

    Sleep(500);

    for (int i = 0; i < AVG_SET; i++) {
        // If we call rdtsc too fast we'll end up with div by 0
        Sleep(200);

        s_rdtsc.QuadPart = __rdtsc();
        s_ticks = KeTickCount;

        s_rdtsc.QuadPart -= f_rdtsc.QuadPart;
        s_rdtsc.QuadPart /= s_ticks - f_ticks;

        f_rdtsc.QuadPart = __rdtsc();
        f_ticks = KeTickCount;

        // Skip the first result as invalid
        if (i)
            avg.QuadPart += s_rdtsc.QuadPart;
    }
    frequency.QuadPart = avg.QuadPart / (AVG_SET - 1) * 1000LL;
}
#endif

BOOL QueryPerformanceCounter (LARGE_INTEGER *lpPerformanceCount)
{
    assert(lpPerformanceCount != NULL);

    lpPerformanceCount->QuadPart = __rdtsc();
    return TRUE;
}

BOOL QueryPerformanceFrequency (LARGE_INTEGER *lpFrequency)
{
    assert(lpFrequency != NULL);

#ifdef USE_RDTSC_FOR_FREQ
    lpFrequency->QuadPart = frequency.QuadPart;
#else
    lpFrequency->QuadPart = 733333333;
#endif
    return TRUE;
}