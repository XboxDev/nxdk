// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#include <profileapi.h>
#include <assert.h>
#include <stddef.h>
#include <xboxkrnl/xboxkrnl.h>

BOOL QueryPerformanceCounter (LARGE_INTEGER *lpPerformanceCount)
{
    assert(lpPerformanceCount != NULL);

    lpPerformanceCount->QuadPart = KeQueryPerformanceCounter();
    return TRUE;
}

BOOL QueryPerformanceFrequency (LARGE_INTEGER *lpFrequency)
{
    assert(lpFrequency != NULL);

    lpFrequency->QuadPart = KeQueryPerformanceFrequency();
    return TRUE;
}
