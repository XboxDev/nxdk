.globl __aullshl
__aullshl:
    shldl %cl, %eax, %edx
    sall %cl, %eax
    testb $32, %cl
    je .l
    movl %eax, %edx
    xorl %eax, %eax
.l:
    ret
