#include <stdio.h>
#include "_print.h"

int vsnprintf (char * XLIBC_RESTRICT s, size_t n, const char * XLIBC_RESTRICT format, va_list arg)
{
    /* TODO: This function should interpret format as multibyte characters.  */
    struct _XLIBC_status_t status;
    status.base = 0;
    status.flags = 0;
    status.n = n;
    status.i = 0;
    status.current = 0;
    status.s = s;
    status.width = 0;
    status.prec = 0;
    status.stream = NULL;
    va_copy( status.arg, arg );

    while ( *format != '\0' )
    {
        const char * rc;
        if ( ( *format != '%' ) || ( ( rc = _XLIBC_print( format, &status ) ) == format ) )
        {
            /* No conversion specifier, print verbatim */
            if ( status.i < n )
            {
                s[ status.i ] = *format;
            }
            status.i++;
            format++;
        }
        else
        {
            /* Continue parsing after conversion specifier */
            format = rc;
        }
    }
    if ( status.i  < n )
    {
        s[ status.i ] = '\0';
    }
    va_end( status.arg );
    return status.i;
}
