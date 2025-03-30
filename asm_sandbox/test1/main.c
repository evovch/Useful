#include <stdint.h>
#include <stdio.h>
#include "functions_c.h"
#include "functions_asm.h"

int main(int argc, char** argv)
{
    const int32_t a = 3;
    const int32_t b = 5;

    const int32_t res_add_numbers_32_c = add_numbers_32_c(a, b);
    printf("add_numbers_32_c: %d\n", res_add_numbers_32_c);
    const int32_t res_add_numbers_32_asm = add_numbers_32_asm(a, b);
    printf("add_numbers_32_asm: %d\n", res_add_numbers_32_asm);
    const int32_t res_add_numbers_32_asm2 = add_numbers_32_asm2(a, b);
    printf("add_numbers_32_asm2: %d\n", res_add_numbers_32_asm2);

    const int32_t res_mul_numbers_32_c = mul_numbers_32_c(a, b);
    printf("mul_numbers_32_c: %d\n", res_mul_numbers_32_c);
    const int32_t res_mul_numbers_32_asm = mul_numbers_32_asm(a, b);
    printf("mul_numbers_32_asm: %d\n", res_mul_numbers_32_asm);
    const int32_t res_mul_numbers_32_asm2 = mul_numbers_32_asm2(a, b);
    printf("mul_numbers_32_asm2: %d\n", res_mul_numbers_32_asm2);

    const int64_t aa = 3;
    const int64_t bb = 5;

    const int64_t res_add_numbers_64_c = add_numbers_64_c(aa, bb);
    printf("add_numbers_64_c: %ld\n", res_add_numbers_64_c);
    const int64_t res_add_numbers_64_asm = add_numbers_64_asm(aa, bb);
    printf("add_numbers_64_asm: %ld\n", res_add_numbers_64_asm);
    const int64_t res_add_numbers_64_asm2 = add_numbers_64_asm2(aa, bb);
    printf("add_numbers_64_asm2: %ld\n", res_add_numbers_64_asm2);

    const int64_t res_mul_numbers_64_c = mul_numbers_64_c(aa, bb);
    printf("mul_numbers_64_c: %ld\n", res_mul_numbers_64_c);
    const int64_t res_mul_numbers_64_asm = mul_numbers_64_asm(aa, bb);
    printf("mul_numbers_64_asm: %ld\n", res_mul_numbers_64_asm);
    const int64_t res_mul_numbers_64_asm2 = mul_numbers_64_asm2(aa, bb);
    printf("mul_numbers_64_asm2: %ld\n", res_mul_numbers_64_asm2);

    return 0;
}
