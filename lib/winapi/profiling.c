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
static LARGE_INTEGER frequency = {{0, 0}};
static void __attribute__((constructor)) PrimeQueryPerformanceFrequency ()
{
    ULARGE_INTEGER start_rdtsc = {{0, 0}}, end_rdtsc = {{0, 0}};
    ULONG start_ticks = 0, end_ticks = 0;

    KeEnterCriticalRegion();

    // The values generated after launching aren't accurate, give it time to increment...
    Sleep(700);

    start_rdtsc.QuadPart = __rdtsc();
    start_ticks = KeTickCount;

    Sleep(200);

    end_rdtsc.QuadPart = __rdtsc();
    end_ticks = KeTickCount;

    end_rdtsc.QuadPart -= start_rdtsc.QuadPart;
    end_rdtsc.QuadPart /= end_ticks - start_ticks;

    frequency.QuadPart = end_rdtsc.QuadPart;
    frequency.QuadPart *= 1000LL;

    KeLeaveCriticalRegion();
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
