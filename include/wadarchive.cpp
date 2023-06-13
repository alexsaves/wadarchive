#include "wadarchive.hpp"
#include "wadentry.hpp"

#include "utils.hpp"
#include "constants.hpp"

#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

namespace wadarchive
{
	/// @brief Create a wad writer with a specific file
	/// @param destination_file The location of the file
	WadArchiveWriter::WadArchiveWriter(string destination_file)
	{
		file_location = destination_file;

		// Check if the file exists
		if (utils::file_exists(file_location))
		{
			// delete the file
			remove(file_location.c_str());
		}

		string str = string(ENGINE_NAME) + "[" + utils::get_double_as_string(ENGINE_VERSION, 2) + "]";
		const int bufferSize = 30;
		char buffer[bufferSize];
		char fillChar = ' ';
    	std::fill_n(buffer, bufferSize, fillChar);

		std::copy(str.begin(), str.end(), buffer);
		
		// Create an empty file
		// Open the file in binary mode
		std::ofstream file(file_location, std::ios::binary);

		if (file.is_open())
		{
			// Write the character array to the file
			file.write(buffer, bufferSize);

			// Close the file
			file.close();
		}
	}

	/// @brief Destroy the writer
	WadArchiveWriter::~WadArchiveWriter() {}

	/// @brief Wrap up the file
	void WadArchiveWriter::close()
	{
		// no op
	}

	/// @brief Add a new file entry to the wad
	/// @param filename The name of the entry
	/// @param data The data to be written
	WadEntry *WadArchiveWriter::AddFile(string filename, char *data, int datalen)
	{
		return NULL;
	}
}