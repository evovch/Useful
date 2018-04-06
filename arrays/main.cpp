// Compile: g++ main.cpp -o prog -std=c++11

#include <cmath>
#include <iostream>
#include <ccomplex>

using std::cout;
using std::endl;

typedef std::complex<double> tComplex;

int main(int argc, char** argv)
{
	unsigned long int amount[25];
	tComplex* arr[25]; // static array of 25 addresses

	unsigned int i;

	for (i=0; i<25; i++) {
		amount[i] = pow(2, i+1);
		arr[i] = new tComplex[amount[i]];
	}

	for (i=0; i<25; i++) {
		cout << i << ":\t" << amount[i] << endl;
	}

	// Don't forget to free the memory
	for (i=0; i<25; i++) {
		delete [] arr[i];
	}

	return 0;
}