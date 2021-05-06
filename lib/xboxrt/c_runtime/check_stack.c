#include <errhandlingapi.h>
#include <hal/debug.h>
#include <xboxkrnl/xboxkrnl.h>

#ifdef DEBUG_CONSOLE
    #define _print DbgPrint
#else
    #define _print debugPrint
#endif

void _cdecl _xlibc_check_stack (DWORD requested_size, DWORD stack_ptr)
{
    PKTHREAD current_thread = KeGetCurrentThread();

    if (requested_size >= stack_ptr || stack_ptr - requested_size < (DWORD)current_thread->StackLimit)
    {
        _print("\n"
               "Stack overflow caught!\n"
               "stack pointer: 0x%lx\n"
               "request size:  0x%lx\n"
               "stack limit:   0x%lx\n"
               "\n",
               stack_ptr, requested_size, (DWORD)current_thread->StackLimit);

        RaiseException(EXCEPTION_STACK_OVERFLOW, EXCEPTION_NONCONTINUABLE, 0, NULL);
    }
}
