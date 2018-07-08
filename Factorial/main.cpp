#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define TYPE2

#ifndef TYPE2

unsigned int Factorial(unsigned int p)
{
	if (p==0 || p==1) return 1;
	unsigned int rez = 1;
	for (unsigned int i=2; i<=p; i++) {
		rez *= i;
	}
	//printf("%d! = %d\n", p, rez);
	return rez;
}

unsigned int coef1(unsigned int n, unsigned int k)
{
	return Factorial(n) / (Factorial(k) * Factorial(n-k));
}

#endif

#ifdef TYPE2

unsigned int coef2(unsigned int n, unsigned int k)
{
	unsigned int nom = 1;
	unsigned int denom = 1;
	if (k > (n-k)) {
		for (unsigned int i=k+1; i<=n; i++) {
			nom *= i;
		}
		for (unsigned int i=1; i<=n-k; i++) {
			denom *= i;
		}
	} else {
		for (unsigned int i=(n-k)+1; i<=n; i++) {
			nom *= i;
		}
		for (unsigned int i=1; i<=k; i++) {
			denom *= i;
		}
	}
	return nom/denom;
}

#endif

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Please specify number of events.\n");
		return 1;
	}

	unsigned long int nEvents = atoi(argv[1]);
	unsigned int n_max = 10;
	srand(time(NULL));

	for (unsigned long int iEvent=0; iEvent<nEvents; iEvent++) {
		unsigned int n = rand() % (n_max-1) + 1;
		unsigned int k = rand() % n;
		#ifndef TYPE2
		unsigned int c1 = coef1(n, k);
		#endif
		#ifdef TYPE2
		unsigned int c2 = coef2(n, k);
		#endif
		//printf("%ld: n=%d, k=%d, c1=%d, c2=%d\n", iEvent, n, k, c1, c2);
		/*signed int diff = c2-c1;
		if (diff != 0) {
			printf("ERROR\n");
		}*/
	}
	return 0;
}