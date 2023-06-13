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

		/// @brief The compressed size
		int compressed_size;

		/// @brief Create a new empty instance
		WadEntry();

		/// @brief Destroy the entry
		~WadEntry();
	};
}

#endif