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
    #define NV_PRAMDAC_PLL_COEFF *(volatile ULONG *)0xFD680500
    #define NV_PTIMER_NUM *(volatile ULONG *)0xFD009200
    #define NV_PTIMER_DEN *(volatile ULONG *)0xFD009210
    #define NV_PTIMER_COUNT 0xFD009400
    #define ASM_LOOPS 1024 * 4

    ULARGE_INTEGER rdtsc_count_1 = {{0, 0}}, rdtsc_count_2 = {{0, 0}};
    DWORD ptimer_count_1 = 0, ptimer_count_2 = 0;

    // Precalcuate NVCLK & PTIMER freq
    double nv_clock = BASE_CLOCK_FLOAT * ((NV_PRAMDAC_PLL_COEFF & 0xFF00) >> 8);
    nv_clock /= 1 << ((NV_PRAMDAC_PLL_COEFF & 0x70000) >> 16);
    nv_clock /= NV_PRAMDAC_PLL_COEFF & 0xFF;

    double ptimer_frequency = (nv_clock / NV_PTIMER_NUM) * NV_PTIMER_DEN;

    KeEnterCriticalRegion();

    __asm
    {
        push eax
        push edx
        push ecx

        cli
        sfence

        // Turn off caches
        mov eax, cr0
        or eax, 1 << 30 // Set CD bit
        mov cr0, eax
        wbinvd

        // Reset PTIMER
        mov eax, [NV_PTIMER_COUNT]
        and eax, ~(0xFFFFFFE0) // First 5 bits are not used
        mov [NV_PTIMER_COUNT], eax

        rdtsc
        mov rdtsc_count_1.LowPart, eax
        mov rdtsc_count_1.HighPart, edx
        
        mov eax, [NV_PTIMER_COUNT]
        mov ptimer_count_1, eax

        // Spin for a bit
        mov eax, ASM_LOOPS
        loop_1:
        dec eax
        jnz loop_1

        rdtsc
        mov rdtsc_count_2.LowPart, eax
        mov rdtsc_count_2.HighPart, edx
        
        mov eax, [NV_PTIMER_COUNT]
        mov ptimer_count_2, eax

        // Without this, invaldidating the cache below will crash the system
        sfence

        mov eax, cr0
        and eax, ~(1 << 30) // Clear CD bit
        mov cr0, eax
        wbinvd

        sti

        pop ecx
        pop edx
        pop eax
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
