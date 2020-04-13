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
