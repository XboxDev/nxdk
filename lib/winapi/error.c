// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#include <threads.h>
#include <winbase.h>

static thread_local DWORD lastError = 0;

DWORD GetLastError (void)
{
    return lastError;
}

void SetLastError (DWORD error)
{
    lastError = error;
}
