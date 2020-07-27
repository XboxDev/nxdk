#ifdef __cplusplus
extern "C" {
#endif

char *strdup (const char *s);

static char *_strdup (const char *s)
{
    return strdup(s);
}

// Compared to their Win32 counterparts, these functions currently don't invoke
// an invalid parameter handler routine as described in the MSDN, and instead
// behave as if the invalid parameter handler allowed to continue execution.
int _strnicmp (const char *s1, const char *s2, size_t n);
int _stricmp (const char *s1, const char *s2);

__attribute__((deprecated)) static int strnicmp (const char *s1, const char *s2, size_t n)
{
    return _strnicmp(s1, s2, n);
}

__attribute__((deprecated)) static int stricmp (const char *s1, const char *s2)
{
    return _stricmp(s1, s2);
}

#ifndef NLSCMP_DEFINED
#define _NLSCMPERROR 0x7FFFFFFF
#define _NLSCMP_DEFINED
#endif

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
// libc++ expects MSVCRT to provide these overloads

#ifndef _NXDK_PREFERRED_OVERLOAD
#define _NXDK_PREFERRED_OVERLOAD __attribute__ ((__enable_if__(true, "")))
#endif

inline static char *__nxdk_strchr (const char *__s, int __c)
{
    return static_cast<char *>(strchr(__s, __c));
}

inline static _NXDK_PREFERRED_OVERLOAD const char *strchr (const char *__s, int __c)
{
    return __nxdk_strchr(__s, __c);
}

inline static _NXDK_PREFERRED_OVERLOAD char *strchr (char *__s, int __c)
{
    return __nxdk_strchr(__s, __c);
}

inline static char *__nxdk_strpbrk (const char *__s1, const char *__s2)
{
    return static_cast<char *>(strpbrk(__s1, __s2));
}

inline static _NXDK_PREFERRED_OVERLOAD const char *strpbrk (const char *__s1, const char *__s2)
{
    return __nxdk_strpbrk(__s1, __s2);
}

inline static _NXDK_PREFERRED_OVERLOAD char *strpbrk (char *__s1, char *__s2)
{
    return __nxdk_strpbrk(__s1, __s2);
}

inline static char *__nxdk_strrchr (const char *__s, int __c)
{
    return static_cast<char *>(strrchr(__s, __c));
}

inline static _NXDK_PREFERRED_OVERLOAD const char *strrchr (const char *__s, int __c)
{
    return __nxdk_strrchr(__s, __c);
}

inline static _NXDK_PREFERRED_OVERLOAD char *strrchr (char *__s, int __c)
{
    return __nxdk_strrchr(__s, __c);
}

inline static void *__nxdk_memchr (const void *__s, int __c, size_t __n)
{
    return static_cast<void *>(memchr(__s, __c, __n));
}

inline static _NXDK_PREFERRED_OVERLOAD const void *memchr (const void *__s, int __c, size_t __n)
{
    return __nxdk_memchr(__s, __c, __n);
}

inline static _NXDK_PREFERRED_OVERLOAD void *memchr (void *__s, int __c, size_t __n)
{
    return __nxdk_memchr(__s, __c, __n);
}

inline static char *__nxdk_strstr (const char *__s1, const char *__s2)
{
    return static_cast<char *>(strstr(__s1, __s2));
}

inline static _NXDK_PREFERRED_OVERLOAD const char *strstr (const char *__s1, const char *__s2)
{
    return __nxdk_strstr(__s1, __s2);
}

inline static _NXDK_PREFERRED_OVERLOAD char *strstr (char *__s1, const char *__s2)
{
    return __nxdk_strstr(__s1, __s2);
}

#endif
