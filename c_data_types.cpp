#include <stdio.h>

void foo(char)          { printf("%llu - char\n",          sizeof(char)); }
void foo(signed char)   { printf("%llu - signed char\n",   sizeof(signed char)); }
void foo(unsigned char) { printf("%llu - unsigned char\n", sizeof(unsigned char)); }

void foo(int)           { printf("%llu - int\n",          sizeof(int)); }
//void foo(signed int)  { printf("%llu - signed int\n",   sizeof(signed int)); }                                // signed int ~ int
void foo(unsigned int)  { printf("%llu - unsigned int\n", sizeof(unsigned int)); }

///*
void foo(short int)          { printf("%llu - short int\n",          sizeof(short int)); }
//void foo(signed short int) { printf("%llu - signed short int\n",   sizeof(signed short int)); }               // signed short int ~ short int
void foo(unsigned short int) { printf("%llu - unsigned short int\n", sizeof(unsigned short int)); }

void foo(long int)           { printf("%llu - long int\n",          sizeof(long int)); }
//void foo(signed long int)  { printf("%llu - signed long int\n",   sizeof(signed long int)); }                 // signed long int ~ long int
void foo(unsigned long int)  { printf("%llu - unsigned long int\n", sizeof(unsigned long int)); }

void foo(long long int)          { printf("%llu - long long int\n", sizeof(long long int)); }
//void foo(signed long long int) { printf("%llu - signed long long int\n", sizeof(signed long long int)); }     // signed long long int ~ long long int
void foo(unsigned long long int) { printf("%llu - unsigned long long int\n", sizeof(unsigned long long int)); }
//*/

/*
// 'int' word can be skipped if either 'short', 'long' or 'long long' keywords are used

void foo(short)          { printf("%llu - short\n", sizeof(short)); }                   //          short ~          short int
//void foo(signed short) { printf("%llu - signed short\n", sizeof(signed short)); }     //   signed short ~   signed short int
void foo(unsigned short) { printf("%llu - unsigned short\n", sizeof(unsigned short)); } // unsigned short ~ unsigned short int

void foo(long)           { printf("%llu - long\n", sizeof(long)); }                     //          long  ~          long  int
//void foo(signed long)  { printf("%llu - signed long\n", sizeof(signed long)); }       // signed   long  ~   signed long  int
void foo(unsigned long)  { printf("%llu - unsigned long\n", sizeof(unsigned long)); }   // unsigned long  ~ unsigned long  int

void foo(long long)          { printf("%llu - long long\n", sizeof(long long)); }                   //          long long ~          long long int
//void foo(signed long long) { printf("%llu - signed long long\n", sizeof(signed long long)); }     // signed   long long ~   signed long long int
void foo(unsigned long long) { printf("%llu - unsigned long long\n", sizeof(unsigned long long)); } // unsigned long long ~ unsigned long long int
//*/

int main()
{
    // CHAR ==============================================================

    char c = 'a'; // Char literal is a symbol in single quotes
    signed char sc = 1; // There is no special suffix for char in the language
    unsigned char uc = 1U; // There is no special suffix for char in the language

    foo('a');  // char
    foo(c);    // char
    foo(sc);   // signed char
    foo(uc);   // unsigned char

    // SHORT INT, INT ====================================================

    short int si = 1; // There is no special suffix for short in the language
    unsigned short int usi = 1U; // There is no special suffix for short in the language
    int i = 1; // No suffix is assumbed to be int ~ signed int
    unsigned int ui = 1U;

    foo(si);  // short int ~ signed short int
    foo(usi); // unsigned short int
    foo(1);   // int ~ signed int
    foo(i);   // int ~ signed int
    foo(1U);  // unsigned int
    foo(ui);  // unsigned int

    // LONG INT, LONG LONG INT ===========================================

    // Everything about long and long long is easy.
    // You can explicitly specify the type of the literal using suffixes

    long int li = 1L;
    unsigned long int uli = 1UL;
    long long int lli = 1LL;
    unsigned long long int ulli = 1ULL;

    foo(1L);   // long ~ signed long
    foo(li);   // long ~ signed long
    foo(1UL);  // unsigned long
    foo(uli);  // unsigned long
    foo(1LL);  // long long ~ signed long long
    foo(lli);  // long long ~ signed long long
    foo(1ULL); // unsigned long long
    foo(ulli); // unsigned long long

    return 0;
}
