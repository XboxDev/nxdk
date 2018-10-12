.text

/*
Buffer layout:
     0: unsigned long ebp
     4: unsigned long ebx
     8: unsigned long edi
    12: unsigned long esi
    16: unsigned long esp
    20: unsigned long eip
*/

.globl _setjmp
_setjmp:
    movl 4(%esp), %eax

    movl %ebp, (%eax)
    movl %ebx, 4(%eax)
    movl %edi, 8(%eax)
    movl %esi, 12(%eax)
    leal 4(%esp), %ecx
    movl %ecx, 16(%eax)
    movl (%esp), %ecx
    movl %ecx, 20(%eax)

    xorl %eax, %eax
    ret
