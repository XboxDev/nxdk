// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2020 Jannik Vogel

#include <windows.h>

BOOL IsBadWritePtr (LPVOID lp, UINT_PTR ucb)
{
    // Windows appears to implement this by registering an SEH handler.
    // The Microsoft implementation then probes each page.
    // For Xbox, this is not suitable because `lp` potentially points at MMIO.

    //FIXME: Walk allocations in region with NtQueryVirtualMemory and check write permission?

    // We disallow all access for now, because memory is potentially unsafe
    return TRUE;
}
