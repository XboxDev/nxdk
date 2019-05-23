#include <winbase.h>
#include <winnt.h>

#include <string.h>

void WINAPI OutputDebugStringA (LPCTSTR lpOutputString)
{
    ANSI_STRING s;

    s.Buffer = (LPSTR)lpOutputString;
    s.Length = (USHORT)strlen(s.Buffer);
    s.MaximumLength = s.Length + 1;

    __asm__ __volatile__("mov $1, %%eax\n" // $1 = BREAKPOINT_PRINT
                         "int $0x2D\n"
                         "int $3\n": :"c" (&s));
}
