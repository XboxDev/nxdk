#ifndef _XLIBC_STDIO_H
#define _XLIBC_STDIO_H

#include <stdarg.h>
#include <_restrict.h>
#include <_unimplemented.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _XLIBC_SIZE_T_DEFINED
#define _XLIBC_SIZE_T_DEFINED _XLIBC_SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

#ifndef _XLIBC_NULL_DEFINED
#define _XLIBC_NULL_DEFINED _XLIBC_NULL_DEFINED
#define NULL ((void *)0)
#endif

#define BUFSIZ 256

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3

#define EOF -1

typedef struct {
    // FIXME
} FILE;

typedef struct _xlibc_fpos
{
    // FIXME
} fpos_t;

int remove (const char *filename) __unimplemented;
int rename (const char *old, const char *new) __unimplemented;

FILE *tmpfile (void) __unimplemented;
char *tmpnam(char *s) __unimplemented;

int fclose (FILE *stream) __unimplemented;
int fflush (FILE *stream) __unimplemented;
FILE *fopen (const char * XLIBC_RESTRICT filename, const char * XLIBC_RESTRICT mode) __unimplemented;
FILE *freopen (const char * XLIBC_RESTRICT filename, const char * XLIBC_RESTRICT mode, FILE * XLIBC_RESTRICT stream) __unimplemented;
void setbuf (FILE * XLIBC_RESTRICT stream, char * XLIBC_RESTRICT buf) __unimplemented;
int setvbuf (FILE * XLIBC_RESTRICT stream, char * XLIBC_RESTRICT buf, int mode, size_t size) __unimplemented;
int fprintf (FILE * XLIBC_RESTRICT stream, const char * XLIBC_RESTRICT format, ...) __unimplemented;
int fscanf (FILE * XLIBC_RESTRICT stream, const char * XLIBC_RESTRICT format, ...) __unimplemented;

int printf (const char * XLIBC_RESTRICT format, ...) __unimplemented;
int scanf (const char * XLIBC_RESTRICT format, ...) __unimplemented;
int snprintf (char * XLIBC_RESTRICT s, size_t n, const char * XLIBC_RESTRICT format, ...);
int sprintf (char * XLIBC_RESTRICT s, const char * XLIBC_RESTRICT format, ...);
int sscanf (const char * XLIBC_RESTRICT s, const char * XLIBC_RESTRICT format, ...) __unimplemented;
int vfprintf (FILE * XLIBC_RESTRICT stream, const char * XLIBC_RESTRICT format, va_list arg) __unimplemented;
int vfscanf (FILE * XLIBC_RESTRICT stream, const char * XLIBC_RESTRICT format, va_list arg) __unimplemented;
int vprintf (const char * XLIBC_RESTRICT format, va_list arg) __unimplemented;
int vscanf (const char * XLIBC_RESTRICT format, va_list arg) __unimplemented;
int vsnprintf (char * XLIBC_RESTRICT s, size_t n, const char * XLIBC_RESTRICT format, va_list arg);
int vsprintf (char * XLIBC_RESTRICT s, const char * XLIBC_RESTRICT format, va_list arg);
int vsscanf (const char * XLIBC_RESTRICT s, const char * XLIBC_RESTRICT format, va_list arg) __unimplemented;
int fgetc (FILE *stream) __unimplemented;
char *fgets (char * XLIBC_RESTRICT s, int n, FILE * XLIBC_RESTRICT stream) __unimplemented;
int fputc (int c, FILE *stream) __unimplemented;
int fputs (const char * XLIBC_RESTRICT s, FILE * XLIBC_RESTRICT stream) __unimplemented;
int getc (FILE *stream) __unimplemented;
int getchar (void) __unimplemented;
int putc (int c, FILE *stream) __unimplemented;
int putchar (int c) __unimplemented;
int ungetc (int c, FILE *stream) __unimplemented;

size_t fread (void * XLIBC_RESTRICT ptr, size_t size, size_t nmemb, FILE * XLIBC_RESTRICT stream) __unimplemented;
size_t fwrite (const void * XLIBC_RESTRICT ptr, size_t size, size_t nmemb, FILE * XLIBC_RESTRICT stream) __unimplemented;
int fgetpos (FILE * XLIBC_RESTRICT stream, fpos_t * XLIBC_RESTRICT pos) __unimplemented;
int fseek (FILE *stream, long int offset, int whence) __unimplemented;
int fsetpos(FILE *stream, const fpos_t *pos) __unimplemented;
long int ftell (FILE *stream) __unimplemented;
void rewind (FILE *stream) __unimplemented;
void clearerr (FILE *stream) __unimplemented;
int feof (FILE *stream) __unimplemented;
int ferror (FILE *stream) __unimplemented;
void perror (const char *s) __unimplemented;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _XLIBC_STDIO_H */
