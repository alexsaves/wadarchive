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
		byte_location = 0;
	}

	/// @brief Create a new instance with data
	/// @param fname The name of the file
	/// @param data The data
	/// @param filelen The data length
	WadEntry::WadEntry(string fname, char *data, int filelen)
	{
		file_name = fname;
		size = filelen;
		file_data = data;
	}

	/// @brief Destroy the entry
	WadEntry::~WadEntry() {
		if (file_data != NULL) {
			free(file_data);
			file_data = NULL;
		}
	}
}
