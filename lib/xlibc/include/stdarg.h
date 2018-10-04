#ifndef _XLIBC_STDARG_H
#define _XLIBC_STDARG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef __builtin_va_list va_list;

#define va_start(v,l) __builtin_va_start((v), (l))
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg
#define va_copy(d,s) __builtin_va_copy((d),(s))

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _XLIBC_STDARG_H */
