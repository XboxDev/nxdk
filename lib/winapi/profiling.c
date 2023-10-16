// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#include <profileapi.h>
#ifdef USE_RDTSC_FOR_FREQ
#include <synchapi.h>
#endif
#include <assert.h>
#include <stddef.h>
#include <xboxkrnl/xboxkrnl.h>

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
    #define AVG_SET 10
    ULARGE_INTEGER f_rdtsc, avg;
    ULONG f_ticks = 0;

    avg.QuadPart = 0;

    for (int i = 0; i < AVG_SET; i++) {
        ULARGE_INTEGER s_rdtsc;
        ULONG s_ticks;

        s_rdtsc.QuadPart = __rdtsc();
        s_ticks = KeTickCount;

        s_rdtsc.QuadPart -= f_rdtsc.QuadPart;
        s_rdtsc.QuadPart /= s_ticks - f_ticks;

        f_rdtsc.QuadPart = __rdtsc();
        f_ticks = KeTickCount;

        // Skip the first result as invalid
        if (i)
            avg.QuadPart += s_rdtsc.QuadPart;
        
        // If we call rdtsc too fast we'll end up with div by 0
        Sleep(10);
    }
    lpFrequency->QuadPart = (avg.QuadPart / (AVG_SET - 1)) * 1000;
#else
    lpFrequency->QuadPart = 733333333;
#endif
    return TRUE;
}