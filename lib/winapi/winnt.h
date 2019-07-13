#ifndef __WINNT_H__
#define __WINNT_H__

#include <xboxkrnl/xboxdef.h>

typedef LONG HRESULT;

typedef CHAR *LPSTR;

#ifdef UNICODE
typedef LPCWSTR LPCTSTR;
#else
typedef LPCSTR LPCTSTR;
#endif

#endif
