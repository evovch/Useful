#include <stdio.h>

int main(int argc, char** argv)
{
	bool b;
	unsigned char uc;
	signed char sc;
	char c;
	unsigned short int usi;
	signed short int ssi;
	short int shi;
	unsigned int ui;
	signed int si;
	int i;
	unsigned long int uli;
	signed long int sli;
	long int li;
	unsigned long long int ulli;
	signed long long int slli;
	long long int lli;
	float f;
	double d;
	long double ld;

	bool* pb;
	unsigned char* puc;
	signed char* psc;
	char* pc;
	unsigned short int* pusi;
	signed short int* pssi;
	short int* pshi;
	unsigned int* pui;
	signed int* psi;
	int* pi;
	unsigned long int* puli;
	signed long int* psli;
	long int* pli;
	unsigned long long int* pulli;
	signed long long int* pslli;
	long long int* plli;
	float* pf;
	double* pd;
	long double* pld;

	printf ("bool:\t\t\t%ld\n", sizeof(b));
	printf ("unsigned char:\t\t%ld\nsigned char:\t\t%ld\nchar:\t\t\t%ld\n", sizeof(uc), sizeof(sc), sizeof(c));
	printf ("unsigned short int:\t%ld\nsigned short int:\t%ld\nshort int:\t\t%ld\n", sizeof(usi), sizeof(ssi), sizeof(shi));
	printf ("unsigned int:\t\t%ld\nsigned int:\t\t%ld\nint:\t\t\t%ld\n", sizeof(ui), sizeof(si), sizeof(i));
	printf ("unsigned long int:\t%ld\nsigned long int:\t%ld\nlong int:\t\t%ld\n", sizeof(uli), sizeof(sli), sizeof(li));
	printf ("unsigned long long int:\t%ld\nsigned long long int:\t%ld\nlong long int:\t\t%ld\n", sizeof(ulli), sizeof(slli), sizeof(lli));


	printf ("float:\t\t%ld\ndouble:\t\t%ld\nlong double:\t%ld\n\n", sizeof(f), sizeof(d), sizeof(ld));

	printf ("Pointers: %ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\n",
                                                                        sizeof(pb),   // bool
                                                                        sizeof(puc),  // unsigned char
                                                                        sizeof(psc),  // signed char
                                                                        sizeof(pc),   // char
                                                                        sizeof(pusi), // unsigned short int
                                                                        sizeof(pssi), // signed short int
                                                                        sizeof(pshi), // short int
                                                                        sizeof(pui),  // unsigned int
                                                                        sizeof(psi),  // signed int
                                                                        sizeof(pi),   // int
                                                                        sizeof(puli), // unsigned long int
                                                                        sizeof(psli), // signed long int
                                                                        sizeof(pli),  // long int
                                                                        sizeof(pulli),// unsigned long long int
                                                                        sizeof(pslli),// signed long long int
                                                                        sizeof(plli), // long long int
                                                                        sizeof(pf),   // float
                                                                        sizeof(pd),   // double
                                                                        sizeof(pld)); // long double
}
