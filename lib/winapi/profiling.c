// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2025 Stefan Schmidt

#include <assert.h>
#include <profileapi.h>
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

    lpFrequency->QuadPart = 733333333;
    return TRUE;
}
