/*
    This file is licensed under the CC0 1.0.
    For details, see: https://creativecommons.org/publicdomain/zero/1.0/
*/

.include "prelude.s.inc"
safeseh_prelude

/*
    __chkstk does not comply to any standardized calling convention.
    When called, eax contains the size of the request, and esp is modified
    accordingly before returning.
    Calling __chkstk has the same effect as "subl %eax, %esp".
*/
.text
.globl __chkstk
__chkstk:
    pushl %ecx
    leal 8(%esp), %ecx  // Load original stack address into ecx
    pushl %ecx          // Save caller-saved registers before calling a C-function
    pushl %eax
    pushl %edx

    pushl %ecx
    pushl %eax
    call __xlibc_check_stack
    addl $8, %esp

    popl %edx
    popl %eax
    popl %ecx
    subl %eax, %ecx     // ecx is the new stack pointer
    leal 4(%esp), %eax  // eax is a pointer to the return address variable
    movl %ecx, %esp     // load the new stack address
    movl -4(%eax), %ecx // restore ecx
    pushl (%eax)        // put return address back on the stack
    subl %esp, %eax     // restore eax

    ret
