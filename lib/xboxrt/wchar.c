#include "wchar.h"

#include <stdint.h>

wchar_t *wcscpy(wchar_t *dst, const wchar_t *src) {
    return wcsncpy(dst, src, SIZE_MAX);
}

wchar_t *wcsncpy(wchar_t *dst, const wchar_t *src, size_t n) {
    size_t i = 0;
    do dst[i] = *src; while (*src++ != L'\0' && i++ < n);
    return dst;
}

wchar_t *wcscat(wchar_t *s1, const wchar_t *s2) {
    return wcsncat(s1, s2, SIZE_MAX);
}

wchar_t *wcsncat(wchar_t *s1, const wchar_t *s2, size_t n) {
    wcsncpy(s1 + wcslen(s1), s2, n);
    return s1;
}

size_t wcslen(const wchar_t *s1) {
    size_t len = 0;
    while(s1[len] != L'\0') { len++; }
    return len;
}

int wcscmp(const wchar_t *s1, const wchar_t *s2) {
    return wcsncmp(s1, s2, SIZE_MAX);
}

int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n) {
    for (size_t pos = 0; pos < n; pos++) {
        wchar_t c1 = *s1++;
        wchar_t c2 = *s2++;
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (c1 == 0 || c2 == 0) break;
    }
    return 0;
}
