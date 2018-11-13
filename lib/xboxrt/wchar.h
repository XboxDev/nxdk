#ifndef XBOXRT_WCHAR
#define XBOXRT_WCHAR

#include <stddef.h>

wchar_t *wcscat(wchar_t *s1, const wchar_t *s2);
wchar_t *wcscpy(wchar_t *dst, const wchar_t *src);
wchar_t *wcsncat(wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wcsncpy(wchar_t *dst, const wchar_t *src, size_t n);
size_t wcslen(const wchar_t *s1);
int wcscmp(const wchar_t *s1, const wchar_t *s2);
int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);

#endif
