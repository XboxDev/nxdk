#pragma once

#if defined(__cplusplus)
extern "C"
{
#endif

#include <stdarg.h>

#include <nxdk/log.h>
#include <nxdk/log_console.h>

#warning hal/debug.h is deprecated. Consider using nxLogPrintf instead.

#define debugPrint(format, ...) { nxLogConsoleRegister(); nxLogPrintf(format, ##__VA_ARGS__); }

#define debugPrintNum(x) debugPrint("%d", x)
#define debugClearScreen nxLogConsoleClear
#define debugAdvanceScreen nxLogConsoleAdvance
#define debugMoveCursor nxLogConsoleMoveCursor
#define debugResetCursor nxLogConsoleMoveCursor(25, 25);

void debugPrintBinary(int num)
{
    int8_t x = 0;
    char binary_number[50] = { 0 };

    for(int8_t i = 31; i >= 0; i--)
    {
        binary_number[x++] = 0x30 + ((num & (0x01 << i))?1:0);

        if((i % 4) == 0) binary_number[x++] = ' ';
    }

    binary_number[x] = 0;
    debugPrint("%s", binary_number);
}

void debugPrintHex(const char* buffer, int length)
{
    for (int32_t i = 0; i < length; i++) debugPrint("0x%02x ", buffer[i]);
}

#if defined(__cplusplus)
}
#endif
