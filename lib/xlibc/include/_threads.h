#ifndef _XLIBC__THREADS_H
#define _XLIBC__THREADS_H

#include "xboxkrnl/xboxkrnl.h"

VOID NTAPI _xlibc_thread_startup (PKSTART_ROUTINE StartRoutine, PVOID StartContext);

#endif /* end of include guard: _XLIBC__THREADS_H */
