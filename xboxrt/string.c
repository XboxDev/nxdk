#include "string.h"
#include "ctype.h"

int memcmp(const void *p1, const void *p2, int num) {
    unsigned char *c1 = (unsigned char *)p1;
    unsigned char *c2 = (unsigned char *)p2;
    for (int i = 0; i < num; i++) {
        if (c1[i] < c2[i]) {
            return -1;
        } else if (c1[i] > c2[i]) {
            return 1;
        }
    }
    return 0;
}

void *memchr(const void *ptr, int c, int n) {
    unsigned const char *cptr = ptr;
    for (int i = 0; i < n; i++) {
        if (cptr[i] == c) {
            return (void *)cptr+i;
        }
    }
    return 0;
}

void *memcpy(void *dst, const void *src, int num) {
    char *cdst = dst;
    const char *csrc = src;
    for (int i = 0; i < num; i++) {
        *cdst++ = *csrc++;
    }
    return dst;
}

void *memmove(void *dst, const void *src, int num) {
    return memcpy(dst, src, num);
}

void *memset(void *ptr, int val, int num) {
    unsigned char *cptr = ptr;
    for (int i = 0; i < num; i++) {
        *cptr++ = val;
    }
    return ptr;
}


size_t strlen(const char *s1) {
    size_t i = 0;
    do i++; while (s1[i] != '\0');
    return i;
}

#if 0
void *strdup(const char *s1) {
    void *out = malloc(strlen(s1));
    strcpy(out, s1);
    return out;
}
#endif

int strcmp(const char *s1, const char *s2) {
    return strncmp(s1, s2, SIZE_MAX);
}

int strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t pos = 0; pos < n; pos++) {
        char c1 = *s1++;
        char c2 = *s2++;
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (c1 == 0 || c2 == 0) break;
    }
    return 0;
}

int stricmp(const char *s1, const char *s2) {
    return strnicmp(s1, s2, SIZE_MAX);
}

int strnicmp(const char *s1, const char *s2, size_t n) {
    for (size_t pos = 0; pos < n; pos++) {
        char c1 = tolower(*s1++);
        char c2 = tolower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (c1 == 0 || c2 == 0) break;
    }
    return 0;
}

char *strcpy(char *dst, const char *src) {
    return strncpy(dst, src, SIZE_MAX);
}

char *strncpy(char *dst, const char *src, size_t n) {
    size_t i = 0;
    do dst[i] = *src; while (*src++ != '\0' && i++ < n);
    return dst;
}

char *strcat(char *s1, const char *s2) {
    return strncat(s1, s2, SIZE_MAX);
}

char *strncat(char *s1, const char *s2, size_t n) {
    strncpy(s1 + strlen(s1), s2, n);
    return s1;
}

char *strupr(char *s1) {
    do *s1 = toupper(*s1); while (*s1++ != '\0');
    return s1;
}

char * strchr(const char * s, int c)
{
    for(; *s != (char) c; ++s)
        if (*s == '\0')
            return NULL;
    return (char *) s;
}

char * strrchr(const char * s, int c)
{
       const char *p = s + strlen(s);
       do {
           if (*p == (char)c)
               return (char *)p;
       } while (--p >= s);
       return NULL;
}
