#ifndef XBOXRT_STAT
#define XBOXRT_STAT

#ifdef __cplusplus
extern "C" {
#endif

#if 0
int fstat(int fd, struct stat *st);
int stat(const char *filename, struct stat *st);
#endif

#include <stdint.h>
typedef uint32_t mode_t;

#ifdef __cplusplus
}
#endif

#endif
