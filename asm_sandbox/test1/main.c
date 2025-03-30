#include <stdint.h>
#include <stdio.h>
#include "functions_c.h"
#include "functions_asm.h"

void print_array(const char* const name, const uint32_t* const values)
{
    printf("%s: {%d, %d, %d, %d}\n", name, values[0], values[1], values[2], values[3]);
}

int main(int argc, char** argv)
{
    const int32_t a_32 = 3;
    const int32_t b_32 = 5;

    const int32_t res_add_ints_32_c = add_ints_32_c(a_32, b_32);
    printf("add_ints_32_c: %d\n", res_add_ints_32_c);
    const int32_t res_add_ints_32_asm = add_ints_32_asm(a_32, b_32);
    printf("add_ints_32_asm: %d\n", res_add_ints_32_asm);
    const int32_t res_add_ints_32_asm2 = add_ints_32_asm2(a_32, b_32);
    printf("add_ints_32_asm2: %d\n", res_add_ints_32_asm2);

    const int32_t res_mul_ints_32_c = mul_ints_32_c(a_32, b_32);
    printf("mul_ints_32_c: %d\n", res_mul_ints_32_c);
    const int32_t res_mul_ints_32_asm = mul_ints_32_asm(a_32, b_32);
    printf("mul_ints_32_asm: %d\n", res_mul_ints_32_asm);
    const int32_t res_mul_ints_32_asm2 = mul_ints_32_asm2(a_32, b_32);
    printf("mul_ints_32_asm2: %d\n", res_mul_ints_32_asm2);

    const int64_t a_64 = 3;
    const int64_t b_64 = 5;

    const int64_t res_add_ints_64_c = add_ints_64_c(a_64, b_64);
    printf("add_ints_64_c: %ld\n", res_add_ints_64_c);
    const int64_t res_add_ints_64_asm = add_ints_64_asm(a_64, b_64);
    printf("add_ints_64_asm: %ld\n", res_add_ints_64_asm);
    const int64_t res_add_ints_64_asm2 = add_ints_64_asm2(a_64, b_64);
    printf("add_ints_64_asm2: %ld\n", res_add_ints_64_asm2);

    const int64_t res_mul_ints_64_c = mul_ints_64_c(a_64, b_64);
    printf("mul_ints_64_c: %ld\n", res_mul_ints_64_c);
    const int64_t res_mul_ints_64_asm = mul_ints_64_asm(a_64, b_64);
    printf("mul_ints_64_asm: %ld\n", res_mul_ints_64_asm);
    const int64_t res_mul_ints_64_asm2 = mul_ints_64_asm2(a_64, b_64);
    printf("mul_ints_64_asm2: %ld\n", res_mul_ints_64_asm2);

    const uint32_t a_32x4[4] = { 1, 2, 3, 4 };
    const uint32_t b_32x4[4] = { 5, 6, 7, 8 };

    uint32_t res_add_ints_32x4_c[4];
    add_ints_32x4_c(a_32x4, b_32x4, res_add_ints_32x4_c);
    print_array("add_ints_32x4_c", res_add_ints_32x4_c);
    uint32_t res_add_ints_32x4_asm[4];
    add_ints_32x4_asm(a_32x4, b_32x4, res_add_ints_32x4_asm);
    print_array("add_ints_32x4_asm", res_add_ints_32x4_asm);

    uint32_t res_mul_ints_32x4_c[4];
    mul_ints_32x4_c(a_32x4, b_32x4, res_mul_ints_32x4_c);
    print_array("mul_ints_32x4_c", res_mul_ints_32x4_c);
    uint32_t res_mul_ints_32x4_asm[4];
    mul_ints_32x4_asm(a_32x4, b_32x4, res_mul_ints_32x4_asm);
    print_array("mul_ints_32x4_asm", res_mul_ints_32x4_asm);

    return 0;
}
