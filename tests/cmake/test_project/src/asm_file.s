.globl _add_ten

# Define the start of the text/code section
.text

_add_ten:
    pushl   %ebp
    movl    %esp, %ebp

    movl    8(%ebp), %eax
    addl    $10, %eax

    leave
    ret
