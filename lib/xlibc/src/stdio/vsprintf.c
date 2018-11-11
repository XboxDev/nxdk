#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

int vsprintf (char * restrict s, const char * restrict format, va_list arg)
{
    return vsnprintf(s, SIZE_MAX, format, arg);
}
