#include "wadentry.hpp"

#include <stdlib.h>
#include <iostream>

using namespace std;

namespace wadarchive
{
	/// @brief Create a new empty instance
	WadEntry::WadEntry()
	{
		size = 0;
		compressed_size = 0;
	}

	/// @brief Destroy the entry
	WadEntry::~WadEntry() {}
}
