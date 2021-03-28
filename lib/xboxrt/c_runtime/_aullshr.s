.include "prelude.s.inc"
safeseh_prelude

.text
.balign 4
.globl __aullshr
__aullshr:
    shrdl %cl, %edx, %eax
    shrl %cl, %edx
    testb $32, %cl
    je .l
    movl %edx, %eax
    xorl %edx, %edx
.l:
    ret
