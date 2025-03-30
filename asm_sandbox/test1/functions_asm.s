    .section .text
    .globl add_ints_32_asm
    .globl add_ints_32_asm2
    .globl mul_ints_32_asm
    .globl mul_ints_32_asm2
    .globl add_ints_64_asm
    .globl add_ints_64_asm2
    .globl mul_ints_64_asm
    .globl mul_ints_64_asm2
    .globl add_ints_32x4_asm
    .globl mul_ints_32x4_asm
    .globl add_floats_asm
    .globl add_floats_asm2
    .globl mul_floats_asm
    .globl mul_floats_asm2
    .globl add_floats_x4_asm
    .globl mul_floats_x4_asm

add_ints_32_asm:
    movl %ecx, %eax  # l suffix in movl, addl, imull - 32 bit operation
    addl %edx, %eax  # e prefix in eax, ecx, edx - use only lower 32 bits of operands
    ret
add_ints_32_asm2:
    pushq %rbp
    movq %rsp, %rbp
    movl %ecx, %eax
    addl %edx, %eax
    popq %rbp
    ret
mul_ints_32_asm:
    movl %ecx, %eax
    imull %edx, %eax
    ret
mul_ints_32_asm2:
    pushq %rbp
    movq %rsp, %rbp
    movl %ecx, %eax
    imull %edx, %eax
    popq %rbp
    ret

add_ints_64_asm:
    movq %rcx, %rax  # q suffix in movq, addq, imulq - 64 bit operation
    addq %rdx, %rax  # r prefix in rax, rcx, rdx - use all 64 bits of operands
    ret
add_ints_64_asm2:
    pushq %rbp
    movq %rsp, %rbp
    movq %rcx, %rax
    addq %rdx, %rax
    popq %rbp
    ret
mul_ints_64_asm:
    movq %rcx, %rax
    imulq %rdx, %rax
    ret
mul_ints_64_asm2:
    pushq %rbp
    movq %rsp, %rbp
    movq %rcx, %rax
    imulq %rdx, %rax
    popq %rbp
    ret

add_ints_32x4_asm:
    movdqu (%rcx), %xmm0
    movdqu (%rdx), %xmm1
    paddd %xmm1, %xmm0
    movdqu %xmm0, (%r8)
    ret
mul_ints_32x4_asm:
    movdqu (%rcx), %xmm0
    movdqu (%rdx), %xmm1
    pmulld %xmm1, %xmm0
    movdqu %xmm0, (%r8)
    ret

add_floats_asm:
    movss %xmm0, %xmm2
    addss %xmm1, %xmm2
    movss %xmm2, %xmm0
    ret
add_floats_asm2:
    addss %xmm1, %xmm0
    ret
mul_floats_asm:
    movss %xmm0, %xmm2
    mulss %xmm1, %xmm2
    movss %xmm2, %xmm0
    ret
mul_floats_asm2:
    mulss %xmm1, %xmm0
    ret

add_floats_x4_asm:
    movaps (%rcx), %xmm0
    movaps (%rdx), %xmm1
    addps %xmm1, %xmm0
    movaps %xmm0, (%r8)
    ret
mul_floats_x4_asm:
    movaps (%rcx), %xmm0
    movaps (%rdx), %xmm1
    mulps %xmm1, %xmm0
    movaps %xmm0, (%r8)
    ret
