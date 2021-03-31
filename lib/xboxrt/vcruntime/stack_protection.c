#include <assert.h>
#define _CRT_RAND_S
#include <stdlib.h>
#include <windows.h>
#include <hal/xbox.h>

#define DEFAULT_SECURITY_COOKIE 0x0BADC0DE

// The stack canary value. A default value is provided, but for proper security a call to __security_init_cookie() is mandatory
UINT_PTR __security_cookie  = DEFAULT_SECURITY_COOKIE;

void __attribute__((no_stack_protector)) __cdecl __security_init_cookie (void)
{
    if (__security_cookie != DEFAULT_SECURITY_COOKIE) {
        return;
    }

    UINT_PTR tmpcookie;
    rand_s(&tmpcookie);
    if (tmpcookie == DEFAULT_SECURITY_COOKIE) {
        tmpcookie++;
    }
    __security_cookie = tmpcookie;
}

void __attribute__((no_stack_protector)) __fastcall __security_check_cookie (UINT_PTR cookie)
{
    assert (cookie == __security_cookie);

    if (cookie == __security_cookie) {
        return;
    }

    DbgPrint("STACK SMASHING DETECTED\ncookie: %x\nexpected: %x", cookie, __security_cookie);
    XReboot();
}
