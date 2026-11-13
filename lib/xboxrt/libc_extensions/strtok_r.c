// SPDX-License-Identifier: MIT

/**
 * @file lib/xboxrt/libc_extensions/strtok_r.c
 * @brief Implement reentrant string tokenization without shared state.
 */
#include <string.h>

char *strtok_r (char *str, const char *delim, char **saveptr)
{
    if (str != NULL) {
        *saveptr = str;
    }
    if (*saveptr == NULL) {
        return NULL;
    }

    char *token = *saveptr + strspn(*saveptr, delim);
    if (*token == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    char *end = token + strcspn(token, delim);
    if (*end == '\0') {
        *saveptr = NULL;
    } else {
        *end = '\0';
        *saveptr = end + 1;
    }
    return token;
}
