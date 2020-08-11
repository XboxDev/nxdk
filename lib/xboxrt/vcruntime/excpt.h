#ifndef __VCRUNTIME_EXCPT_H__
#define __VCRUNTIME_EXCPT_H__

#include <stdint.h>
#include <windows.h>

typedef struct _EXCEPTION_REGISTRATION
{
    struct _EXCEPTION_REGISTRATION *prev;
    void *handler;
} EXCEPTION_REGISTRATION, *PEXCEPTION_REGISTRATION;


#define GetExceptionCode _exception_code

#define EXCEPTION_EXECUTE_HANDLER 1
#define EXCEPTION_CONTINUE_SEARCH 0
#define EXCEPTION_CONTINUE_EXECUTION (-1)

typedef enum _EXCEPTION_DISPOSITION
{
    ExceptionContinueExecution = 0,
    ExceptionContinueSearch = 1,
    ExceptionNestedException = 2,
    ExceptionCollidedUnwind = 3,
} EXCEPTION_DISPOSITION;

#endif
