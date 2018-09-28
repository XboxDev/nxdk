#ifndef _XLIBC_ASSERT_H
#define _XLIBC_ASSERT_H

/* based on _PDCLIB_symbol2value from PDCLIB */
#ifndef _XLIBC_symbol2value
#define _XLIBC_symbol2value(x) #x
#endif

/* based on _PDCLIB_symbol2string from PDCLIB */
#ifndef _XLIBC_symbol2string
#define _XLIBC_symbol2string(x) _XLIBC_symbol2value(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#undef assert
#ifdef NDEBUG
    #define assert(ignore) ((void)0)
#else
    void _xlibc_assert(char const * const expression, char const * const file_name, char const * const function_name, unsigned long line);

    #define assert(expression) \
        do { \
            if(!(expression)) { \
                _xlibc_assert(_XLIBC_symbol2string(expression), \
                              __FILE__, \
                              __func__, \
                              __LINE__); \
            } \
        } while(0)
#endif

#define static_assert _Static_assert

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _XLIBC_ASSERT_H */
