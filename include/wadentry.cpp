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
	WadEntry::WadEntry(string fname, vector<char> data)
	{
		file_name = fname;
		size = data.size();
		file_data = data;
	}

	/// @brief Destroy the entry
	WadEntry::~WadEntry()
	{
		file_data.clear();
	}

	/// @brief Get the data as a null terminated character array
	/// @return A null terminated character array
	vector<char> WadEntry::getData()
	{
		return file_data;
	}

	/// @brief Set the data
	/// @param data The data
	void WadEntry::setData(vector<char> data)
	{
		empty();
		file_data = data;
		size = data.size();
	}

	/// @brief Zeroes out the data
	void WadEntry::empty()
	{
		size = 0;
		file_data.clear();
	}

	/// @brief Free the memory
	void WadEntry::free_just_memory()
	{
		file_data.clear();
	}
}
