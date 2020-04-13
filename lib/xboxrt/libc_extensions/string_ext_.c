#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char *strdup (const char *s)
{
    if (s == NULL) {
        return NULL;
    }

    char *new_s = malloc(strlen(s) + 1);
    if (new_s != NULL) {
        strcpy(new_s, s);
    }

    return new_s;
}

int _strnicmp (const char *s1, const char *s2, size_t n)
{
    assert(s1);
    assert(s2);

    if (!s1 || !s2) {
        errno = EINVAL;
        return _NLSCMPERROR;
    }

    while (n && *s1 && (tolower(*s1) == tolower(*s2))) {
        ++s1;
        ++s2;
        --n;
    }

    if (n == 0) {
        return 0;
    } else {
        return (tolower(*(unsigned char *)s1) - tolower(*(unsigned char *)s2));
    }
}

int _stricmp (const char *s1, const char *s2)
{
    assert(s1);
    assert(s2);

    if (!s1 || !s2) {
        errno = EINVAL;
        return _NLSCMPERROR;
    }

    while (*s1 && (tolower(*s1) == tolower(*s2))) {
        ++s1;
        ++s2;
    }

    return (tolower(*(unsigned char *)s1) - tolower(*(unsigned char *)s2));
}

