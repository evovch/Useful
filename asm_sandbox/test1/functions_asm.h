#ifndef FUNCTIONS_ASM_H
#define FUNCTIONS_ASM_H

#include <stdint.h>

int32_t add_ints_32_asm (int32_t a, int32_t b);
int32_t add_ints_32_asm2(int32_t a, int32_t b);
int32_t mul_ints_32_asm (int32_t a, int32_t b);
int32_t mul_ints_32_asm2(int32_t a, int32_t b);

int64_t add_ints_64_asm (int64_t a, int64_t b);
int64_t add_ints_64_asm2(int64_t a, int64_t b);
int64_t mul_ints_64_asm (int64_t a, int64_t b);
int64_t mul_ints_64_asm2(int64_t a, int64_t b);

void add_ints_32x4_asm(const int32_t* const a, const int32_t* const b, int32_t* const res);
void mul_ints_32x4_asm(const int32_t* const a, const int32_t* const b, int32_t* const res);

float add_floats_asm (float a, float b);
float add_floats_asm2(float a, float b);
float mul_floats_asm (float a, float b);
float mul_floats_asm2(float a, float b);

void add_floats_x4_asm(const float* const a, const float* const b, float* const res);
void mul_floats_x4_asm(const float* const a, const float* const b, float* const res);

#endif // FUNCTIONS_ASM_H
