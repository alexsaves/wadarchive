#pragma once

#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

namespace wadarchive
{
	class WadEntry
	{
	public:
		/// @brief The name of the file
		string file_name;

		/// @brief The uncompressed file size
		size_t size;

		/// @brief The location of the file
		int byte_location;

		/// @brief Create a new empty instance
		WadEntry();

		/// @brief Create a new instance with data
		/// @param fname The name of the file
		/// @param data The data
		/// @param filelen The data length
		WadEntry(string fname, vector<char> data);

		/// @brief Destroy the entry
		~WadEntry();

		/// @brief Get the data as a null terminated character array
		/// @return A null terminated character array
		vector<char> getData();

		/// @brief Set the data
		/// @param data The data
		void setData(vector<char> data);

		/// @brief Zeroes out the data
		void empty();

		/// @brief Release just the memory but don't change anything else
		void free_just_memory();

	private:
		/// @brief The uncompressed data
		vector<char> file_data;
	};
}