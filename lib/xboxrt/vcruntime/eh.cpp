// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2020 Stefan Schmidt

#include <assert.h>
#include <eh.h>

void __cdecl __ExceptionPtrCreate (void*)
{
    assert(0);
}

void __cdecl __ExceptionPtrDestroy (void*)
{
    assert(0);
}

void __cdecl __ExceptionPtrCopy (void*, const void*)
{
    assert(0);
}

void __cdecl __ExceptionPtrAssign (void*, const void*)
{
    assert(0);
}

bool __cdecl __ExceptionPtrCompare (const void*, const void*)
{
    assert(0);
    return false;
}

bool __cdecl __ExceptionPtrToBool (const void*)
{
    assert(0);
    return false;
}

void __cdecl __ExceptionPtrSwap (void*, void*)
{
    assert(0);
}

void __cdecl __ExceptionPtrCurrentException (void*)
{
    assert(0);
}

[[noreturn]] void __cdecl __ExceptionPtrRethrow (const void*)
{
    assert(0);
}

void __cdecl __ExceptionPtrCopyException (void*, const void*, const void*)
{
    assert(0);
}

extern "C"
{
    terminate_handler __cdecl set_terminate (terminate_handler _NewTerminateHandler) throw()
    {
        assert(0);
        return 0;
    }

    terminate_handler __cdecl _get_terminate ()
    {
        assert(0);
        return 0;
    }

    unexpected_handler __cdecl set_unexpected (unexpected_handler _NewUnexpectedHandler) throw()
    {
        assert(0);
        return 0;
    }

    unexpected_handler __cdecl _get_unexpected ()
    {
        assert(0);
        return 0;
    }

    int __cdecl __uncaught_exceptions ()
    {
        assert(0);
        return 0;
    }
}
