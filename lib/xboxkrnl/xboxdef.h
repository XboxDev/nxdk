#ifndef __XBOXDEF_H__
#define __XBOXDEF_H__

typedef const void *LPCVOID;
typedef void VOID, *PVOID, *LPVOID;
typedef PVOID HANDLE, *PHANDLE;

typedef unsigned char BOOLEAN, *PBOOLEAN;

typedef signed char SCHAR, *PSCHAR;

typedef char CHAR, *PCHAR, CCHAR, *LPCH, *PCH, OCHAR, *POCHAR;
typedef short SHORT, *PSHORT;
typedef long LONG, *PLONG;
typedef long long LONGLONG, *PLONGLONG;

typedef unsigned char UCHAR, *PUCHAR;
typedef unsigned short USHORT, *PUSHORT, CSHORT;
typedef unsigned short WORD, WCHAR, *PWSTR;
typedef unsigned int DWORD, *PDWORD, *LPDWORD;
typedef unsigned long ULONG, *PULONG;
typedef unsigned long long ULONGLONG;

typedef unsigned int SIZE_T, *PSIZE_T;

typedef int BOOL, *PBOOL;
typedef const char *PCSZ, *PCSTR, *LPCSTR;

typedef ULONG ULONG_PTR;
typedef LONG LONG_PTR;

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR  Buffer;
} STRING, *PSTRING;

typedef STRING ANSI_STRING, *PANSI_STRING;

/**
 * MS's way to represent a 64-bit signed int on platforms that may not support
 * them directly.
 */
typedef union _LARGE_INTEGER
{
    struct
    {
        ULONG LowPart; /**< The low-order 32 bits. */
        LONG HighPart; /**< The high-order 32 bits. */
    };
    struct
    {
        ULONG LowPart; /**< The low-order 32 bits. */
        LONG HighPart; /**< The high-order 32 bits. */
    } u;
    LONGLONG QuadPart; /**< A signed 64-bit integer. */
} LARGE_INTEGER, *PLARGE_INTEGER;

/**
 * MS's way to represent a 64-bit unsigned int on platforms that may not support
 * them directly.
 */
typedef union _ULARGE_INTEGER
{
    struct
    {
        ULONG LowPart; /**< The low-order 32 bits. */
        ULONG HighPart; /**< The high-order 32 bits. */ /**< The high-order 32 bits. */
    };
    struct
    {
        ULONG LowPart; /**< The low-order 32 bits. */
        ULONG HighPart; /**< The high-order 32 bits. */
    } u;
    ULONGLONG QuadPart; /**< An unsigned 64-bit integer. */
} ULARGE_INTEGER, *PULARGE_INTEGER;

#endif
