.globl __allshl
__allshl:
    shldl %cl, %eax, %edx
    sall %cl, %eax
    testb $32, %cl
    je .l
    movl %eax, %edx
    xorl %eax, %eax
.l:
    ret
