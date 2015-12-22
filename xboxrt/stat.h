#ifndef XBOXRT_STAT
#define XBOXRT_STAT

#if 0
int fstat(int fd, struct stat *st);
int stat(const char *filename, struct stat *st);
#endif

#include <stdint.h>
typedef uint32_t mode_t;

#endif