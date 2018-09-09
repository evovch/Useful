#include <iostream>
using std::cout;
using std::endl;

#include "Support.h"

int main(int argc, char** argv)
{
	cout << "=============================================" << endl;
	cout << "unsigned short overflow demonstration" << endl;
	unsigned short us = 65534; cout << us << endl;
	us++;                      cout << us << endl;
	us++;                      cout << us << endl;
	us++;                      cout << us << endl;

	cout << "=============================================" << endl;
	cout << "normal loop demonstration" << endl;
	for (signed short i=10; i>=0; i--) {
		cout << i << endl;
	}

	cout << "=============================================" << endl;
	cout << "infinite loop demonstration" << endl;

	unsigned short counter = 0;  // safety counter
	for (unsigned short i=10; i>=0; i--) {
		cout << i << endl;
		if (counter++ > 30) {
			break;
		}
	}

	cout << "=============================================" << endl;
	cout << "bitwise operations demonstration" << endl;

	unsigned int rawWord = 0xa83b0136;

	cout << support::GetHexRepresentation(sizeof(unsigned int), (void*)&rawWord) << endl;
	cout << rawWord << endl;
	cout << support::GetBinaryRepresentation(sizeof(unsigned int), (void*)&rawWord) << endl;
}
