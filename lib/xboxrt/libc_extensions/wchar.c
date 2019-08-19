#include "wchar.h"

wchar_t * wcscat( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2 )
{
    wchar_t * rc = s1;
    if ( *s1 )
    {
        while ( *++s1 );
    }
    while ( (*s1++ = *s2++) );
    return rc;
}

wchar_t *wcschr(const wchar_t * haystack, wchar_t needle)
{
    while(*haystack) {
        if(*haystack == needle) return (wchar_t*) haystack;
        haystack++;
    }
    return NULL;
}

int wcscmp( const wchar_t * s1, const wchar_t * s2 )
{
    while ( ( *s1 ) && ( *s1 == *s2 ) )
    {
        ++s1;
        ++s2;
    }
    return ( *(wchar_t *)s1 - *(wchar_t *)s2 );
}

int wcscoll( const wchar_t * s1, const wchar_t * s2 )
{
    return wcscmp(s1, s2);
}

wchar_t *wcscpy( wchar_t * XBOXRT_RESTRICT dest, const wchar_t * XBOXRT_RESTRICT src)
{
    wchar_t * rv = dest;
    while( (*dest++ = *src++) );
    return rv;
}

size_t wcscspn( const wchar_t * s1, const wchar_t * s2 )
{
    size_t len = 0;
    const wchar_t * p;
    while ( s1[len] )
    {
        p = s2;
        while ( *p )
        {
            if ( s1[len] == *p++ )
            {
                return len;
            }
        }
        ++len;
    }
    return len;
}

size_t wcslen( const wchar_t * str )
{
    size_t n = 0;
    while(*(str++)) n++;
    return n;
}

wchar_t * wcsncat( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2, size_t n )
{
    wchar_t * rc = s1;
    while ( *s1 )
    {
        ++s1;
    }
    while ( n && ( *s1++ = *s2++ ) )
    {
        --n;
    }
    if ( n == 0 )
    {
        *s1 = '\0';
    }
    return rc;
}

int wcsncmp( const wchar_t * s1, const wchar_t * s2, size_t n )
{
    while ( *s1 && n && ( *s1 == *s2 ) )
    {
        ++s1;
        ++s2;
        --n;
    }
    if ( n == 0 )
    {
        return 0;
    }
    else
    {
        return ( *(wchar_t *)s1 - *(wchar_t *)s2 );
    }
}

wchar_t *wcsncpy( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2, size_t n )
{
    wchar_t * rc = s1;
    while ( ( n > 0 ) && ( *s1++ = *s2++ ) )
    {
        /* Cannot do "n--" in the conditional as size_t is unsigned and we have
           to check it again for >0 in the next loop below, so we must not risk
           underflow.
        */
        --n;
    }
    /* Checking against 1 as we missed the last --n in the loop above. */
    while ( n-- > 1 )
    {
        *s1++ = '\0';
    }
    return rc;
}

wchar_t * wcspbrk( const wchar_t * s1, const wchar_t * s2 )
{
    const wchar_t * p1 = s1;
    const wchar_t * p2;
    while ( *p1 )
    {
        p2 = s2;
        while ( *p2 )
        {
            if ( *p1 == *p2++ )
            {
                return (wchar_t *) p1;
            }
        }
        ++p1;
    }
    return NULL;
}

wchar_t *wcsrchr(const wchar_t * haystack, wchar_t needle)
{
    wchar_t *found = NULL;
    while(*haystack) {
        if(*haystack == needle) found = (wchar_t*) haystack;
        haystack++;
    }
    return found;
}

size_t wcsspn( const wchar_t * s1, const wchar_t * s2 )
{
    size_t len = 0;
    const wchar_t * p;
    while ( s1[ len ] )
    {
        p = s2;
        while ( *p )
        {
            if ( s1[len] == *p )
            {
                break;
            }
            ++p;
        }
        if ( ! *p )
        {
            return len;
        }
        ++len;
    }
    return len;
}

wchar_t * wcsstr( const wchar_t * s1, const wchar_t * s2 )
{
    const wchar_t * p1 = s1;
    const wchar_t * p2;
    while ( *s1 )
    {
        p2 = s2;
        while ( *p2 && ( *p1 == *p2 ) )
        {
            ++p1;
            ++p2;
        }
        if ( ! *p2 )
        {
            return (wchar_t *) s1;
        }
        ++s1;
        p1 = s1;
    }
    return NULL;
}

wchar_t * wcstok( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2, wchar_t ** XBOXRT_RESTRICT ptr )
{
    const wchar_t * p = s2;

    if ( s1 != NULL )
    {
        /* new string */
        *ptr = s1;
    }
    else
    {
        /* old string continued */
        if ( *ptr == NULL )
        {
            /* No old string, no new string, nothing to do */
            return NULL;
        }
        s1 = *ptr;
    }

    /* skipping leading s2 characters */
    while ( *p && *s1 )
    {
        if ( *s1 == *p )
        {
            /* found seperator; skip and start over */
            ++s1;
            p = s2;
            continue;
        }
        ++p;
    }

    if ( ! *s1 )
    {
        /* no more to parse */
        return ( *ptr = NULL );
    }

    /* skipping non-s2 characters */
    *ptr = s1;
    while ( **ptr )
    {
        p = s2;
        while ( *p )
        {
            if ( **ptr == *p++ )
            {
                /* found seperator; overwrite with '\0', position *ptr, return */
                *(*ptr)++ = L'\0';
                return s1;
            }
        }
        ++(*ptr);
    }

    /* parsed to end of string */
    *ptr = NULL;
    return s1;
}

size_t wcsxfrm( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2, size_t n )
{
    wcsncpy(s1, s2, n);
    return wcslen(s2);
}

wchar_t * wmemchr( const wchar_t * p, wchar_t c, size_t n )
{
    while ( n-- )
    {
        if ( *p == c )
        {
            return (wchar_t*) p;
        }
        ++p;
    }
    return NULL;
}

int wmemcmp( const wchar_t * p1, const wchar_t * p2, size_t n )
{
    while ( n-- )
    {
        if ( *p1 != *p2 )
        {
            return *p1 - *p2;
        }
        ++p1;
        ++p2;
    }
    return 0;
}

wchar_t * wmemcpy( wchar_t * XBOXRT_RESTRICT dest, const wchar_t * XBOXRT_RESTRICT src, size_t n )
{
    wchar_t* rv = dest;
    while ( n-- )
    {
        *dest++ = *src++;
    }
    return rv;
}

wchar_t * wmemmove( wchar_t * dest, const wchar_t * src, size_t n )
{
    wchar_t* rv = dest;
    if ( dest <= src )
    {
        while ( n-- )
        {
            *dest++ = *src++;
        }
    }
    else
    {
        src += n;
        dest += n;
        while ( n-- )
        {
            *--dest = *--src;
        }
    }
    return rv;
}
