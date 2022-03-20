// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2018-2021 Stefan Schmidt

.include "prelude.s.inc"
safeseh_prelude

.text
.balign 4
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
