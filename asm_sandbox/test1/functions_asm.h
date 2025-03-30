#ifndef FUNCTIONS_ASM_H
#define FUNCTIONS_ASM_H

#include <stdint.h>

int32_t add_numbers_32_asm (int32_t a, int32_t b);
int32_t add_numbers_32_asm2(int32_t a, int32_t b);
int32_t mul_numbers_32_asm (int32_t a, int32_t b);
int32_t mul_numbers_32_asm2(int32_t a, int32_t b);

int64_t add_numbers_64_asm (int64_t a, int64_t b);
int64_t add_numbers_64_asm2(int64_t a, int64_t b);
int64_t mul_numbers_64_asm (int64_t a, int64_t b);
int64_t mul_numbers_64_asm2(int64_t a, int64_t b);

#endif // FUNCTIONS_ASM_H
