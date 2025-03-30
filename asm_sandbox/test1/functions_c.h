#ifndef FUNCTIONS_C_H
#define FUNCTIONS_C_H

#include <stdint.h>

int32_t add_ints_32_c(const int32_t a, const int32_t b);
int32_t mul_ints_32_c(const int32_t a, const int32_t b);

int64_t add_ints_64_c(const int64_t a, const int64_t b);
int64_t mul_ints_64_c(const int64_t a, const int64_t b);

void add_ints_32x4_c(const int32_t* const a, const int32_t* const b, int32_t* const res);
void mul_ints_32x4_c(const int32_t* const a, const int32_t* const b, int32_t* const res);

#endif // FUNCTIONS_C_H
