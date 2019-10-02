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
