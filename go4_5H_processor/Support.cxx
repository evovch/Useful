#include "Support.h"

#include <cstdio> // for sprintf

std::string support::GetBinaryRepresentation(size_t const size, void const * const ptr)
{
	std::string outString;

	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;

	char cStr[2];
	cStr[1] = '\0';

	for (int i=size-1; i>=0; i--) {
		for (int j=7; j>=0; j--) {
			byte = (b[i] >> j) & 1;
			sprintf(cStr, "%u", byte);
			outString.append(cStr);
		}
	}

	return outString;
}

std::string support::GetHexRepresentation(size_t const size, void const * const ptr)
{
	std::string outString;

	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;

	char cStr[3];
	cStr[2] = '\0';

	for (int i=size-1; i>=0; i--) {
		byte = b[i] & 0xff;
		sprintf(cStr, "%02x", byte);
		outString.append(cStr);
	}

	return outString;
}
