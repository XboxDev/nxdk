#ifndef _XLIBC_SETJMP_H
#define _XLIBC_SETJMP_H

#ifdef __cplusplus
extern "C" {
#endif

/*
Buffer layout:
     0: unsigned long ebp
     4: unsigned long ebx
     8: unsigned long edi
    12: unsigned long esi
    16: unsigned long esp
    20: unsigned long eip
*/
typedef unsigned long jmp_buf[6];

int setjmp (jmp_buf env);
_Noreturn void longjmp (jmp_buf env, int val);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: _XLIBC_SETJMP_H */
