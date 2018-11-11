#include <stdio.h>
#include <stdarg.h>

int sprintf (char * restrict s, const char * restrict format, ...)
{
    va_list al;
    va_start(al, format);
    int rv = vsprintf(s, format, al);
    va_end(al);
    return rv;
}
