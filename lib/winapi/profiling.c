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
    #define BASE_CLOCK_FLOAT 16.666667f
    #define NV_PRAMDAC_PLL_COEFF *(volatile ULONG*)0xFD680500
    #define NV_PTIMER_NUM *(volatile ULONG*)0xFD009200
    #define NV_PTIMER_DEN *(volatile ULONG*)0xFD009210
    #define NV_PTIMER_COUNT *(volatile ULONG*)0xFD009400
    #define KE_STALL 10

    ULARGE_INTEGER rdtsc_count_1 = {{0, 0}}, rdtsc_count_2 = {{0, 0}};
    DWORD ptimer_count_1 = 0, ptimer_count_2 = 0;

    // Precalcuate NVCLK & PTIMER freq
    double nv_clock = BASE_CLOCK_FLOAT * ((NV_PRAMDAC_PLL_COEFF & 0xFF00) >> 8);
    nv_clock /= 1 << ((NV_PRAMDAC_PLL_COEFF & 0x70000) >> 16);
    nv_clock /= NV_PRAMDAC_PLL_COEFF & 0xFF;

    double ptimer_frequency = (nv_clock / NV_PTIMER_NUM) * NV_PTIMER_DEN;

    KeEnterCriticalRegion();

    // Turn off caches
    __asm
    {
        cli
        sfence
        mov eax, cr0
        or eax, 1 << 30 // Set CD bit
        mov cr0, eax
        wbinvd

    }

    // Reset the counter
    NV_PTIMER_COUNT &= ~(0xFFFFFFE0); // First 5 bits are not used

    rdtsc_count_1.QuadPart = __rdtsc();
    ptimer_count_1 = NV_PTIMER_COUNT;

    KeStallExecutionProcessor(KE_STALL);

    rdtsc_count_2.QuadPart = __rdtsc();
    ptimer_count_2 = NV_PTIMER_COUNT;

    __asm
    {
        sfence
        mov eax, cr0
        and eax, ~(1 << 30) // Clear CD bit
        mov cr0, eax
        wbinvd
        sti
    }

    KeLeaveCriticalRegion();

    double ptimer_diff = (ptimer_count_2 >> 5) - (ptimer_count_1 >> 5);
    double rdtsc_diff = rdtsc_count_2.QuadPart - rdtsc_count_1.QuadPart;

    double ptimer_scale = ptimer_diff / ptimer_frequency;
    double cpu_freq_float = rdtsc_diff / ptimer_scale;

    if (!cpu_freq_float) {
        frequency.QuadPart = 733333333;
    } else {
        frequency.QuadPart = (ULONG)(cpu_freq_float * 1000 * 1000);
    }
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
