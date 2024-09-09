// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Jannik Vogel
// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#include <windows.h>
#include <winnt.h>

#include <string.h>

BOOL IsDebuggerPresent (VOID)
{
    // FIXME: Can we detect debugger presence on the Xbox?
    return FALSE;
}

void WINAPI OutputDebugStringA (LPCTSTR lpOutputString)
{
    ANSI_STRING s;

    s.Buffer = (LPSTR)lpOutputString;
    s.Length = (USHORT)strlen(s.Buffer);
    s.MaximumLength = s.Length + 1;

    __asm__ __volatile__("mov $1, %%eax\n" // $1 = BREAKPOINT_PRINT
                         "int $0x2D\n"
                         "int $3\n" : : "c"(&s));
}
