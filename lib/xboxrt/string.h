#ifndef XBOXRT_STRING
#define XBOXRT_STRING

#include <stddef.h>
#include <stdint.h>

int memcmp(const void *p1, const void *p2, size_t num);
void *memchr(const void *ptr, int c, int n);
void *memcpy(void *dst, const void *src, size_t num);
void *memmove(void *dst, const void *src, size_t num);
void *memset(void *ptr, int val, size_t num);

char *strcat(char *s1, const char *s2);
char *strcpy(char *dst, const char *src);
char *strdup(const char *s1);
char *strncat(char *s1, const char *s2, size_t n);
char *strncpy(char *dst, const char *src, size_t n);
char *strupr(char *s1);
size_t strlen(const char *s1);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
int stricmp(const char *s1, const char *s2);
int strnicmp(const char *s1, const char *s2, size_t n);

char * strchr(const char * s, int c);
char * strrchr(const char * s, int c);

char *strtok( char * s1, const char * s2 );
char *strtok_r(char *str, const char *delim, char **nextp);

#endif
