#ifndef SUPPORT_H
#define SUPPORT_H

// STD
#include <string>

namespace support
{
	/**
	 * Assumes little endian //TODO not sure
	 * size in bytes
	 */
	std::string GetBinaryRepresentation(size_t const size, void const * const ptr);

	/**
	 * size in bytes
	 */
	std::string GetHexRepresentation(size_t const size, void const * const ptr);

}


#endif // SUPPORT_H
