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
typedef unsigned long DWORD, *PDWORD, *LPDWORD;
typedef unsigned long ULONG, *PULONG;
typedef unsigned long long ULONGLONG;

typedef unsigned int SIZE_T, *PSIZE_T;

typedef int BOOL, *PBOOL;
typedef const char *PCSZ, *PCSTR, *LPCSTR;

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR  Buffer;
} STRING, *PSTRING;

typedef STRING ANSI_STRING, *PANSI_STRING;

#endif
