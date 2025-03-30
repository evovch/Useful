    .section .text
    .globl add_numbers_32_asm
    .globl add_numbers_32_asm2
    .globl mul_numbers_32_asm
    .globl mul_numbers_32_asm2
    .globl add_numbers_64_asm
    .globl add_numbers_64_asm2
    .globl mul_numbers_64_asm
    .globl mul_numbers_64_asm2

add_numbers_32_asm:
    movl %ecx, %eax  # l suffix in movl, addl, imull - 32 bit operation
    addl %edx, %eax  # e prefix in eax, ecx, edx - use only lower 32 bits of operands
    ret
add_numbers_32_asm2:
    pushq %rbp
    movq %rsp, %rbp
    movl %ecx, %eax
    addl %edx, %eax
    popq %rbp
    ret
mul_numbers_32_asm:
    movl %ecx, %eax
    imull %edx, %eax
    ret
mul_numbers_32_asm2:
    pushq %rbp
    movq %rsp, %rbp
    movl %ecx, %eax
    imull %edx, %eax
    popq %rbp
    ret

add_numbers_64_asm:
    movq %rcx, %rax  # q suffix in movq, addq, imulq - 64 bit operation
    addq %rdx, %rax  # r prefix in rax, rcx, rdx - use all 64 bits of operands
    ret
add_numbers_64_asm2:
    pushq %rbp
    movq %rsp, %rbp
    movq %rcx, %rax
    addq %rdx, %rax
    popq %rbp
    ret
mul_numbers_64_asm:
    movq %rcx, %rax
    imulq %rdx, %rax
    ret
mul_numbers_64_asm2:
    pushq %rbp
    movq %rsp, %rbp
    movq %rcx, %rax
    imulq %rdx, %rax
    popq %rbp
    ret
