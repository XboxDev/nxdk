#include <stdio.h>
#include <stdarg.h>

int snprintf (char * restrict s, size_t n, const char * restrict format, ...)
{
    va_list al;
    va_start(al, format);
    int rv = vsnprintf(s, n, format, al);
    va_end(al);
    return rv;
}
