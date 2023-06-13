#ifndef WARCHIVE_WAENTRY_H
#define WARCHIVE_WAENTRY_H
#include <stdlib.h>
#include <iostream>

using namespace std;

namespace wadarchive
{
	class WadEntry
	{
	public:
		/// @brief The name of the file
		string file_name;

		/// @brief The uncompressed file size
		int size;

		/// @brief The location of the file
		int byte_location;

		/// @brief The uncompressed data
		char *file_data;

		/// @brief Create a new empty instance
		WadEntry();

		/// @brief Create a new instance with data
		/// @param fname The name of the file
		/// @param data The data
		/// @param filelen The data length
		WadEntry(string fname, char * data, int filelen);

		/// @brief Destroy the entry
		~WadEntry();
	};
}

#endif