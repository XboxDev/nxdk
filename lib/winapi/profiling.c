// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2025 Stefan Schmidt

#include <assert.h>
#include <intrin.h>
#include <profileapi.h>
#include <stddef.h>
#include <stdint.h>
#include <xboxkrnl/xboxkrnl.h>

static LARGE_INTEGER tsc_freq = {.QuadPart = 733333333};

BOOL QueryPerformanceCounter (LARGE_INTEGER *lpPerformanceCount)
{
    assert(lpPerformanceCount != NULL);

    lpPerformanceCount->QuadPart = __rdtsc();
    return TRUE;
}

BOOL QueryPerformanceFrequency (LARGE_INTEGER *lpFrequency)
{
    assert(lpFrequency != NULL);

    *lpFrequency = tsc_freq;
    return TRUE;
}

static uint64_t get_cpu_multiplier (void)
{
    // Read EBL_CR_POWERON
    const uint64_t poweron_reg = __readmsr(0x2A);

    // Chapter 2.22 of https://www.intel.com/content/dam/develop/external/us/en/documents/335592-sdm-vol-4.pdf
    const uint64_t multiplier_pattern = (poweron_reg >> 22) & 0b101111;

    // https://web.archive.org/web/20040407043044/http://www.intel.com:80/design/mobile/datashts/29834006.pdf
    // clang-format off
    switch (multiplier_pattern) {
        case 0b000001: return 30;
        case 0b000101: return 35;
        case 0b000010: return 40;
        case 0b000110: return 45;
        case 0b000000: return 50;
        case 0b000100: return 55;
        case 0b001011: return 60;
        case 0b001111: return 65;
        case 0b001001: return 70;
        case 0b001101: return 75;
        case 0b001010: return 80;
        case 0b100110: return 85;
        case 0b100000: return 90;
        case 0b100100: return 95;
        case 0b101011: return 100;
        case 0b101111: return 105;
        case 0b101010: return 130;
        case 0b101100: return 140;
        // This table is incomplete, the following CPUs and their multipliers are not covered:
        // 110: Coppermine Celeron 733 SL4P3 SL5E9 SL4P7 SL52Y
        // 115: Coppermine Celeron 766 SL4P6 SL4QF SL52X SL5EA
        // 120: Tualatin Celeron 1200 SL5XS SL5Y5 SL656 SL68P SL6C8 SL6JS SL6RP
        // 150: Tualatin Celeron 1500 SL6C5
        default: return 0;
    }
    // clang-format on
}

// Gets run by the CRT on startup
static __cdecl VOID tsc_freq_init (VOID)
{
    // NB clock input frequency, in 0.001 Hz units for precision
    const uint64_t xtal_freq_mhz = 16666666666;

    // CPU-internal clock multiplier, in 0.1 units due to non-integer values
    const uint64_t multiplier_10x = get_cpu_multiplier();

    if (multiplier_10x == 0) {
        // Could not determine clock multiplier, fall back to default TSC freq
        return;
    }

    uint32_t fsbreg;
    HalReadWritePCISpace(0, 0x60, 0x6C, &fsbreg, sizeof(fsbreg), FALSE);
    const uint64_t fsb_divider = fsbreg & 0xFF;
    const uint64_t fsb_multiplier = (fsbreg >> 8) & 0xFF;
    if (fsb_divider == 0 || fsb_multiplier == 0) {
        return;
    }
    const uint64_t fsb_freq = (xtal_freq_mhz / fsb_divider) * fsb_multiplier;
    const uint64_t cpu_freq = fsb_freq * multiplier_10x / 10 / 1000;

    tsc_freq.QuadPart = cpu_freq;
}

#pragma comment(linker, "/include:___tsc_freq_init_p")
__attribute__((section(".CRT$XXT"), used)) void(__cdecl *const __tsc_freq_init_p)(void) = tsc_freq_init;
