/*
 * Copyright (c) 2019 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#include <stdbool.h>
#include <stdlib_ext_.h>
#include <windows.h>
#include <hal/debug.h>

_purecall_handler current_purecall_handler = NULL;

int __cdecl _purecall (void)
{
    if (current_purecall_handler) {
        current_purecall_handler();
    }

    debugPrint("Pure virtual function called!\n");

    // Make sure the handler doesn't return
    while(true) {
        Sleep(1000);
    }

    // return value is not documented by MS (unreachable anyway)
    return 0;
}

_purecall_handler __cdecl _get_purecall_handler (void)
{
    return current_purecall_handler;
}

_purecall_handler __cdecl _set_purecall_handler (_purecall_handler function)
{
    _purecall_handler old_handler = current_purecall_handler;
    current_purecall_handler = function;
    return old_handler;
}
