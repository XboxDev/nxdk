#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include <xboxkrnl/xboxkrnl.h>

static void* VirtualAlloc(void *lpAddress, unsigned int dwSize, unsigned int flAllocationType, unsigned int flProtect)
{
    NtAllocateVirtualMemory(&lpAddress, 0, &dwSize, flAllocationType, flProtect);
    return lpAddress;
}

static int VirtualFree(void *lpAddress, unsigned int dwSize, unsigned int dwFreeType)
{
    return NtFreeVirtualMemory(&lpAddress, &dwSize, dwFreeType);
}

void * malloc(size_t size) {
    return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void free(void *ptr) {
    VirtualFree(ptr, 0, MEM_RELEASE);
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

        // Retrieve the old size
        MEMORY_BASIC_INFORMATION information;
        NtQueryVirtualMemory(ptr, &information);

        // Calculate how many bytes can be copied from old to new buffer
        size_t old_size = information.RegionSize;
        if (old_size > size) {
          old_size = size;
        }

        // Copy data and remove old allocation
        memcpy(new, ptr, old_size);
        free(ptr);
    }
    return new;
}

// This is adapted from libc
// Ref: https://github.com/gcc-mirror/gcc/blob/master/libiberty/strtol.c
long strtol(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

    const long LONG_MAX = 0x7FFFFFFF;
    const long LONG_MIN = 0x80000000;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long) LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
	//	errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

int atoi(const char *nptr) {
	return (int) strtol (nptr, (char **) NULL, 10);
}

int abs (int j)
{
    return (j < 0) ? -j : j;
}
