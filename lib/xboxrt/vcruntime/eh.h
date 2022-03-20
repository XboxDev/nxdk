// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2020 Stefan Schmidt

#ifndef __VCRUNTIME_EH_H__
#define __VCRUNTIME_EH_H__

extern "C"
{
    typedef void (__cdecl *terminate_handler)();
    terminate_handler __cdecl set_terminate (terminate_handler _NewTerminateHandler) throw();
    terminate_handler __cdecl _get_terminate ();

    typedef void (__cdecl *unexpected_handler)();
    unexpected_handler __cdecl set_unexpected (unexpected_handler _NewUnexpectedHandler) throw();
    unexpected_handler __cdecl _get_unexpected ();
}

#endif
