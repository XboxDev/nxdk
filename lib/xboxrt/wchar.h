#ifndef _XBOXRT_WCHAR_H
#define _XBOXRT_WCHAR_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
    #define XBOXRT_RESTRICT __restrict
#else
    #define XBOXRT_RESTRICT restrict
#endif

/* Created from _PDCLIB_mbstate_t */
typedef struct XBOXRT_mbstate {
    union {
        uint64_t _St64[15];
        uint32_t _St32[31];
        uint16_t _St16[62];
        unsigned char    _StUC[124];
        signed   char    _StSC[124];
                 char    _StC [124];
    };
    uint16_t      _Surrogate;
    unsigned char _PendState;
             char _PendChar;
} XBOXRT_mbstate_t;

typedef XBOXRT_mbstate_t mbstate_t;

#ifdef __cplusplus
extern "C" {
#endif

wchar_t * wcscpy( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2 );
wchar_t * wcsncpy( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2, size_t n );
wchar_t * wmemcpy( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2, size_t n );
wchar_t * wmemmove( wchar_t * s1, const wchar_t * s2, size_t n );
wchar_t * wcscat( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2 );
wchar_t * wcsncat( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2, size_t n );
int wcscmp( const wchar_t * s1, const wchar_t * s2 );
int wcscoll( const wchar_t * s1, const wchar_t * s2 );
int wcsncmp( const wchar_t * s1, const wchar_t * s2, size_t n );
size_t wcsxfrm( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2, size_t n );
int wmemcmp( const wchar_t * s1, const wchar_t * s2, size_t n );
wchar_t * wcschr( const wchar_t * s, wchar_t c );
size_t wcscspn( const wchar_t * s1, const wchar_t * s2 );
wchar_t * wcspbrk( const wchar_t * s1, const wchar_t * s2 );
wchar_t * wcsrchr( const wchar_t * s, wchar_t c );
size_t wcsspn( const wchar_t * s1, const wchar_t * s2 );
wchar_t * wcsstr( const wchar_t * s1, const wchar_t * s2 );
wchar_t * wcstok( wchar_t * XBOXRT_RESTRICT s1, const wchar_t * XBOXRT_RESTRICT s2, wchar_t * * XBOXRT_RESTRICT ptr );
wchar_t * wmemchr( const wchar_t * s, wchar_t c, size_t n );
size_t wcslen( const wchar_t * s );
int mbsinit( const mbstate_t * ps );
size_t mbrtowc( wchar_t * XBOXRT_RESTRICT pwc, const char * XBOXRT_RESTRICT s, size_t n, mbstate_t * XBOXRT_RESTRICT ps );
size_t wcrtomb( char * XBOXRT_RESTRICT s, wchar_t wc, mbstate_t * XBOXRT_RESTRICT ps );

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _XBOXRT_WCHAR_H */
