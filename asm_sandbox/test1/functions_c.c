#include "functions_c.h"

int32_t add_ints_32_c(const int32_t a, const int32_t b)
{
    return a + b;
}

int32_t mul_ints_32_c(const int32_t a, const int32_t b)
{
    return a * b;
}

int64_t add_ints_64_c(const int64_t a, const int64_t b)
{
    return a + b;
}

int64_t mul_ints_64_c(const int64_t a, const int64_t b)
{
    return a * b;
}

void add_ints_32x4_c(const int32_t* const a, const int32_t* const b, int32_t* const res)
{
    res[0] = a[0] + b[0];
    res[1] = a[1] + b[1];
    res[2] = a[2] + b[2];
    res[3] = a[3] + b[3];
}

void mul_ints_32x4_c(const int32_t* const a, const int32_t* const b, int32_t* const res)
{
    res[0] = a[0] * b[0];
    res[1] = a[1] * b[1];
    res[2] = a[2] * b[2];
    res[3] = a[3] * b[3];
}

float add_floats_c(const float a, const float b)
{
    return a + b;
}

float mul_floats_c(const float a, const float b)
{
    return a * b;
}

void add_floats_x4_c(const float* const a, const float* const b, float* const res)
{
    res[0] = a[0] + b[0];
    res[1] = a[1] + b[1];
    res[2] = a[2] + b[2];
    res[3] = a[3] + b[3];
}

void mul_floats_x4_c(const float* const a, const float* const b, float* const res)
{
    res[0] = a[0] * b[0];
    res[1] = a[1] * b[1];
    res[2] = a[2] * b[2];
    res[3] = a[3] * b[3];
}
