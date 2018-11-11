#include <stdarg.h>
#include <stdio.h>

static const char _XLIBC_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
static const char _XLIBC_Xdigits[] = "0123456789ABCDEF";

/* Status structure required by _XLIBC_print(). */
struct _XLIBC_status_t
{
    int              base;   /* base to which the value shall be converted   */
    long             flags;  /* flags and length modifiers                   */
    unsigned         n;      /* print: maximum characters to be written      */
                             /* scan:  number matched conversion specifiers  */
    unsigned         i;      /* number of characters read/written            */
    unsigned         current;/* chars read/written in the CURRENT conversion */
    char *           s;      /* *sprintf(): target buffer                    */
                             /* *sscanf():  source string                    */
    unsigned         width;  /* specified field width                        */
    int              prec;   /* specified field precision                    */
    FILE           * stream; /* *fprintf() / *fscanf() stream                */
    va_list          arg;    /* argument stack                               */
};

const char * _XLIBC_print (const char * spec, struct _XLIBC_status_t * status);
