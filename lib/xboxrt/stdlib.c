#include "stdlib.h"
#include "string.h"
#include <xboxkrnl/xboxkrnl.h>

static const char digits[] = "0123456789abcdef";

static unsigned int find_divisor(unsigned long val, unsigned int base)
{
  unsigned int n;

  for (n = 1; val >= base; n *= base, val /= base);

  return n;
}

static unsigned int write_uint(char *dst,
                               unsigned int len,
                               unsigned long val,
                               unsigned int base)
{
  unsigned int div;
  unsigned int n;

  if (base < 2)
    base = 2;
  else if (base > sizeof(digits) - 1)
    base = sizeof(digits) - 1;

  div = find_divisor(val, base); // div < base unless val <= 1
  n = 0;

  while (n < len && div != 0) {
    dst[n++] = digits[val / div];
    val %= div;
    div /= base;
  }

  return n;
}

static unsigned int write_int(char *dst,
                              unsigned int len,
                              long val,
                              unsigned int base)
{
  if (len == 0)
    return 0;

  if (val < 0) {
    *dst = '-';
    return 1 + write_uint(dst + 1, len - 1, -val, base);
  }

  return write_uint(dst, len, val, base);
}

static unsigned int write_string(char *dst, unsigned int len, const char *src)
{
  unsigned int n;
  char c;

  if (src == NULL)
    src = "(null)";

  for (n = 0; n < len; n++) {
    c = src[n];

    if (c == '\0')
      break;

    dst[n] = c;
  }

  return n;
}

int vsnprintf(char *buf, unsigned int len, const char *fmt, va_list ap)
{
  unsigned int i;
  unsigned int n;
  char c;

  if (len == 0)
    return 0;

  for (i = n = 0; n < len; i++) {
    c = fmt[i];

    if (c == '\0')
      break;

    if (c != '%') {
      buf[n++] = c;
      continue;
    }

    c = fmt[++i];

    if (c == 's')
      n += write_string(buf + n, len - n, va_arg(ap, const char *));
    else if (c == 'd')
      n += write_int(buf + n, len - n, va_arg(ap, long), 10);
    else if (c == 'o')
      n += write_uint(buf + n, len - n, va_arg(ap, unsigned long), 8);
    else if (c == 'u')
      n += write_uint(buf + n, len - n, va_arg(ap, unsigned long), 10);
    else if (c == 'p' || c == 'x')
      n += write_uint(buf + n, len - n, va_arg(ap, unsigned long), 16);
    else if (c == 'c')
      buf[n++] = va_arg(ap, int);
    else if (c == '%')
      buf[n++] = '%';
    else
      buf[n++] = c;
  }

  if (n >= len)
    n = len - 1;

  buf[n] = '\0';

  return n;
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
    return vsnprintf(buf, SIZE_MAX, fmt, args);
}

int snprintf(char * str, unsigned int len, const char * fmt, ...) {
    va_list argList;
    va_start(argList, fmt);
    int r = vsnprintf(str, len, fmt, argList);
    va_end(argList);
    return r;
}

int sprintf(char * str, const char * fmt, ...) {
    va_list argList;
    va_start(argList, fmt);
    int r = vsprintf(str, fmt, argList);
    va_end(argList);
    return r;
}


static void* VirtualAlloc(void *lpAddress, unsigned int dwSize, unsigned int flAllocationType, unsigned int flProtect)
{
    NtAllocateVirtualMemory(&lpAddress, 0, &dwSize, flAllocationType, flProtect);
    return lpAddress;
}

static int VirtualFree(void *lpAddress, unsigned int dwSize, unsigned int dwFreeType)
{
    return NtFreeVirtualMemory(lpAddress, &dwSize, dwFreeType);
}

void * malloc(size_t size) {
    return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void free(void *ptr) {
    VirtualFree(ptr, 0, 0);
}

void *calloc(size_t count, size_t size)
{
    void *data = malloc(count * size);
    memset(data, 0, count * size);
    return data;
}

void *realloc(void *ptr, size_t size)
{
    void *new = malloc(size);
    if (ptr != NULL) {
        memcpy(new, ptr, size);
        free(ptr);
    }
    return new;
}
