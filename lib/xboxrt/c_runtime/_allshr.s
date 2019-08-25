.globl __allshr
__allshr:
    shrdl %cl, %edx, %eax
    sarl %cl, %edx
    testb $32, %cl
    je .l
    movl %edx, %eax
    sarl $31, %edx
.l:
    ret
