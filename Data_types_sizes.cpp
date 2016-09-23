#include <stdio.h>

// 1 1 2 2 4 4 8 8 8 8 4 8 8 1
// 8 8 8 8 8 8 8 8 8 8 8 8 8 8

int main(int argc, char** argv)
{
	unsigned char uc;
	signed char sc;
	unsigned short int usi;
	signed short int ssi;
	unsigned int ui;
	signed int si;
	unsigned long int uli;
	signed long int sli;
	unsigned long long int ulli;
	signed long long int slli;
	float f;
	double d;
	long double ld;
	bool b;

	unsigned char* puc;
	signed char* psc;
	unsigned short int* pusi;
	signed short int* pssi;
	unsigned int* pui;
	signed int* psi;
	unsigned long int* puli;
	signed long int* psli;
	unsigned long long int* pulli;
	signed long long int* pslli;
	float* pf;
	double* pd;
	long double* pld;
	bool* pb;

	printf ("unsigned char:\t\t%d\nsigned char:\t\t%d\n", sizeof(uc), sizeof(sc));
	printf ("unsigned short int:\t%d\nsigned short int:\t%d\n", sizeof(usi), sizeof(ssi));
	printf ("unsigned int:\t\t%d\nsigned int:\t\t%d\n", sizeof(ui), sizeof(si));
	printf ("unsigned long int:\t%d\nsigned long int:\t%d\n", sizeof(uli), sizeof(sli));
	printf ("unsigned long long int:\t%d\nsigned long long int:\t%d\n", sizeof(ulli), sizeof(slli));


	printf ("float:\t\t%d\ndouble:\t\t%d\nlong double:\t%d\nbool:\t\t%d\n\n",
                                                                sizeof(f),
                                                                sizeof(d),
                                                                sizeof(ld),
                                                                sizeof(b));

	printf ("Pointers: %d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                                                                        sizeof(puc),
                                                                        sizeof(psc),
                                                                        sizeof(pusi),
                                                                        sizeof(pssi),
                                                                        sizeof(pui),
                                                                        sizeof(psi),
                                                                        sizeof(puli),
                                                                        sizeof(psli),
                                                                        sizeof(pulli),
                                                                        sizeof(pslli),
                                                                        sizeof(pf),
                                                                        sizeof(pd),
                                                                        sizeof(pld),
                                                                        sizeof(pb));
}
