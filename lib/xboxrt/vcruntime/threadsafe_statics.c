// SPDX-License-Identifier: CC0-1.0

// SPDX-FileCopyrightText: 2019 Timo Kreuzer
// SPDX-FileCopyrightText: 2020 Stefan Schmidt

// Derived from ReactOS CRT code to provide thread safe statics initialization
// in nxdk, as required by C++11.

#include <stdint.h>
#include <limits.h>
#include <windows.h>


long _Init_global_epoch = LONG_MIN;
__declspec(thread) long _Init_thread_epoch = LONG_MIN;

/*
    This function tries to acquire a lock on the initialization for the static
    variable by changing the value saved in *ptss to -1. If *ptss is 0, the
    variable was not initialized yet and the function tries to set it to -1.
    If that succeeds, the function will return. If the value is already -1,
    another thread is in the process of doing the initialization and we
    wait for it. If it is any other value the initialization is complete.
    After returning the compiler generated code will check the value:
    if it is -1 it will continue with the initialization, otherwise the
    initialization must be complete and will be skipped.
*/
void _Init_thread_header (volatile int *ptss)
{
    while (1) {
        /* Try to acquire the first initialization lock */
        int oldTss = _InterlockedCompareExchange((long *)ptss, -1, 0);
        if (oldTss == -1) {
            /* Busy, wait for the other thread to do the initialization */
            SwitchToThread();
            continue;
        } else if (oldTss == 0) {
            /* We acquired the lock and the caller will do the initialization */
            return;
        }

        /* The initialization is complete and the caller will skip it.
           Update the epoch so this call can be skipped in the future, it
           only needs to run once per thread. */
        _Init_thread_epoch = _Init_global_epoch;
        return;
    }
}

void _Init_thread_footer (volatile int *ptss)
{
    /* Initialization is complete */
    _Init_thread_epoch = *ptss = _InterlockedIncrement(&_Init_global_epoch);
}

void _Init_thread_abort (volatile int *ptss)
{
    /* Abort the initialization */
    _InterlockedAnd((volatile long *)ptss, 0);
}
