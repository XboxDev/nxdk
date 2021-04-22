#ifndef __WINDEF_H__
#define __WINDEF_H__

#include <xboxkrnl/xboxdef.h>

#define far
#define FAR far

#define WINAPI __stdcall
#define CALLBACK WINAPI

#define MAX_PATH 260

typedef HANDLE HWND;
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;

typedef int (FAR WINAPI *FARPROC)(void);

#endif
