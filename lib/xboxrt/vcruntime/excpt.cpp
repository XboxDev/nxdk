#include <excpt.h>
#include <eh.h>
#include <xboxkrnl/xboxkrnl.h>

struct ScopeTableEntry
{
    DWORD EnclosingLevel;
    void *FilterFunction;
    void *HandlerFunction;
};

struct EXCEPTION_REGISTRATION_SEH3 : EXCEPTION_REGISTRATION
{
    ScopeTableEntry *ScopeTable;
    DWORD TryLevel;
    DWORD _ebp;
};

#define DISPOSITION_DISMISS 0
#define DISPOSITION_CONTINUE_SEARCH 1
#define DISPOSITION_NESTED_EXCEPTION 2
#define DISPOSITION_COLLIDED_UNWIND 3
#define TRYLEVEL_NONE -1

extern "C" int _nested_unwind_handler (_EXCEPTION_RECORD *pExceptionRecord, EXCEPTION_REGISTRATION_SEH3 *pRegistrationFrame, _CONTEXT *pContextRecord, EXCEPTION_REGISTRATION **pDispatcherContext)
{
    if (!(pExceptionRecord->ExceptionFlags & (EXCEPTION_UNWINDING | EXCEPTION_EXIT_UNWIND))) {
        return DISPOSITION_CONTINUE_SEARCH;
    }

    *pDispatcherContext = pRegistrationFrame;
    return DISPOSITION_COLLIDED_UNWIND;
}

extern "C" void _local_unwind2 (EXCEPTION_REGISTRATION_SEH3 *pRegistrationFrame, int stop)
{
    // Manually install exception handler frame
    EXCEPTION_REGISTRATION nestedUnwindFrame;
    nestedUnwindFrame.handler = reinterpret_cast<void *>(_nested_unwind_handler);
    asm volatile (
        "movl %%fs:0, %%eax;"
        "movl %%eax, (%0);"
        "movl %0, %%fs:0;"
        : : "r"(&nestedUnwindFrame) : "eax", "memory");

    const ScopeTableEntry *scopeTable = pRegistrationFrame->ScopeTable;

    while (true) {
        DWORD currentTrylevel = pRegistrationFrame->TryLevel;

        if (currentTrylevel == TRYLEVEL_NONE) {
            break;
        }

        if (stop != TRYLEVEL_NONE && currentTrylevel <= stop) {
            break;
        }

        DWORD oldTrylevel = currentTrylevel;
        pRegistrationFrame->TryLevel = scopeTable[currentTrylevel].EnclosingLevel;

        if (!scopeTable[oldTrylevel].FilterFunction) {
            // If no filter funclet is present, then it's a __finally statement
            // instead of an __except statement
            auto finallyFunclet = reinterpret_cast<void (*)()>(scopeTable[oldTrylevel].HandlerFunction);
            finallyFunclet();
        }
    }

    // Manually remove exception handler frame
    asm volatile (
        "movl (%0), %%eax;"
        "movl %%eax, %%fs:0;"
        : : "r"(&nestedUnwindFrame) : "eax", "memory");
}

extern "C" void _global_unwind2 (EXCEPTION_REGISTRATION_SEH3 *pRegistrationFrame)
{
    asm volatile (
        "pushl $0;"
        "pushl $0;"
        "pushl 1f;"
        "pushl %0;"
        "call _RtlUnwind@16;"
        "1:;"
        : : "r"(pRegistrationFrame));
}

extern "C" int _except_handler3 (_EXCEPTION_RECORD *pExceptionRecord, EXCEPTION_REGISTRATION_SEH3 *pRegistrationFrame, _CONTEXT *pContextRecord, EXCEPTION_REGISTRATION **pDispatcherContext)
{
    // Clear the direction flag - the function triggering the exception might
    // have modified it, but it's expected to not be set
    asm volatile ("cld;");

    if (pExceptionRecord->ExceptionFlags & (EXCEPTION_UNWINDING | EXCEPTION_EXIT_UNWIND)) {
        // We're in an unwinding pass, so unwind all local scopes
        // Unwinding must be done with the ebp of the frame in which we want to unwind
        const DWORD scopeEbp = (DWORD)&pRegistrationFrame->_ebp;
        asm volatile (
            "pushl %%ebp;"
            "movl %0, %%ebp;"
            "pushl %2;"
            "pushl %1;"
            "call __local_unwind2;" // Call _local_unwind2(pRegistrationFrame, TRYLEVEL_NONE);
            "addl $8, %%esp;"
            "popl %%ebp;"
            : : "r"(scopeEbp), "r"(pRegistrationFrame), "i"(TRYLEVEL_NONE) : "eax", "ecx", "edx");

        return DISPOSITION_CONTINUE_SEARCH;
    }

    // A pointer to a EXCEPTION_POINTERS structure needs to be put below below
    // the registration pointer. This is required because the intrinsics
    // implementing GetExceptionInformation() and GetExceptionCode() retrieve
    // their information from this structure.
    EXCEPTION_POINTERS excptPtrs;
    excptPtrs.ExceptionRecord = pExceptionRecord;
    excptPtrs.ContextRecord = pContextRecord;
    reinterpret_cast<PEXCEPTION_POINTERS *>(pRegistrationFrame)[-1] = &excptPtrs;

    const ScopeTableEntry *scopeTable = pRegistrationFrame->ScopeTable;
    DWORD currentTrylevel = pRegistrationFrame->TryLevel;

    // Search all scopes from the inside out trying to find a filter that accepts the exception
    while (currentTrylevel != TRYLEVEL_NONE) {
        const void *filterFunclet = scopeTable[currentTrylevel].FilterFunction;
        if (filterFunclet) {
            const DWORD _ebp = (DWORD)&pRegistrationFrame->_ebp;
            DWORD filterResult;

            asm volatile (
                "pushl %%ebp;"
                "movl %2, %%ebp;"
                "call *%1;"
                "popl %%ebp;"
                : "=a"(filterResult) : "r"(filterFunclet), "r"(_ebp) : "ecx", "edx");

            if (filterResult != EXCEPTION_CONTINUE_SEARCH) {
                if (filterResult == EXCEPTION_CONTINUE_EXECUTION) {
                    return ExceptionContinueExecution;
                }

                // Trigger a second pass through the stack frames, unwinding all scopes
                _global_unwind2(pRegistrationFrame);

                const DWORD scopeEbp = (DWORD)&pRegistrationFrame->_ebp;
                const DWORD newTrylevel = scopeTable[currentTrylevel].EnclosingLevel;
                const DWORD handlerFunclet = (DWORD)scopeTable[currentTrylevel].HandlerFunction;
                asm volatile (
                    "movl %0, %%ebp;"

                    "pushl %%ecx;"
                    "pushl %%ebx;"

                    // Unwind all scopes up to the one handling the exception
                    "pushl %%eax;"
                    "pushl %%edx;"
                    "call __local_unwind2;" // _local_unwind2(pRegistrationFrame, currentTrylevel);
                    "popl %%edx;"
                    "addl $4, %%esp;"

                    "popl %%ebx;"
                    "popl %%ecx;"

                    // Set the new trylevel in EXCEPTION_REGISTRATION_SEH3
                    "movl %%ecx, 12(%%edx);"
                    // Jump into the handler
                    "jmp *%%ebx;"
                    : : "r"(scopeEbp), "a"(currentTrylevel), "b"(handlerFunclet), "c"(newTrylevel), "d"(pRegistrationFrame) : "memory");
            }
        }

        currentTrylevel = pRegistrationFrame->ScopeTable[currentTrylevel].EnclosingLevel;
    }

    // No filter in this frame accepted the exception, continue searching
    return DISPOSITION_CONTINUE_SEARCH;
}
