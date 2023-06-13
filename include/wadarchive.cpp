#include "wadarchive.hpp"
#include "wadentry.hpp"

#include "utils.hpp"
#include "constants.hpp"

#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

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
		const int bufferSize = FRONT_BUFFER_LENGTH;
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
	WadArchiveWriter::~WadArchiveWriter()
	{
		unsigned int entrycount = entries.size();
		for (unsigned int i = 0; i < entrycount; i++)
		{
			WadEntry *wad = entries[i];
			delete wad;
		}
	}

	/// @brief Wrap up the file
	void WadArchiveWriter::close()
	{
		// Write the json blob
		int jsonlocation = utils::filesize(file_location);

		json j;

		j["engine"] = ENGINE_NAME;
		j["engine_ver"] = ENGINE_VERSION;

		json entrylist;

		unsigned int entrycount = entries.size();
		for (unsigned int i = 0; i < entrycount; i++)
		{
			WadEntry *wad = entries[i];
			json wadj;
			wadj["name"] = wad->file_name;
			wadj["size"] = wad->size;
			wadj["location"] = wad->byte_location;
			entrylist.push_back(wadj);
		}

		j["entries"] = entrylist;

		// Dump the json to a char array
		string s = j.dump();
		char buffer[s.length()];
		std::copy(s.begin(), s.end(), buffer);

		// Dump the json location to a buffer
		string jsonlocationstr = utils::get_int_string(jsonlocation);
		const int locbuffersize = SUFFIX_BUFFER_LENGTH;
		char locbuffer[locbuffersize];
		char fillChar = ' ';
		std::fill_n(locbuffer, locbuffersize, fillChar);
		std::copy(jsonlocationstr.begin(), jsonlocationstr.end(), locbuffer);

		std::ofstream file(file_location, std::ios::binary | std::ios::app);

		if (file.is_open())
		{
			// Write the character array to the file
			file.write(buffer, s.length());
			file.write(locbuffer, locbuffersize);

			// Close the file
			file.close();
		}
	}

	/// @brief Add a new file entry to the wad
	/// @param filename The name of the entry
	/// @param data The data to be written
	WadEntry *WadArchiveWriter::AddFile(string filename, char *data, int datalen)
	{
		WadEntry *wad = new WadEntry(filename, NULL, datalen);
		wad->byte_location = utils::filesize(file_location);

		std::ofstream file(file_location, std::ios::binary | std::ios::app);

		if (file.is_open())
		{
			// Write the character array to the file
			file.write(data, datalen);

			// Close the file
			file.close();
		}
		entries.push_back(wad);
		return wad;
	}

	/// @brief Reads a wad archive from the file system
	/// @param source_wad_file The location of the file
	WadArchiveReader::WadArchiveReader(string source_wad_file)
	{
		file_location = source_wad_file;

		// Get file length
		int file_length = utils::filesize(file_location);

		char * jsonposstrchar = utils::read_file_range(file_location.c_str(), file_length - SUFFIX_BUFFER_LENGTH, SUFFIX_BUFFER_LENGTH);

	}

	/// @brief Destroy the reader
	WadArchiveReader::~WadArchiveReader()
	{
		// no-op
	}

	/// @brief Wrap up the reader
	void WadArchiveReader::close()
	{
		// NO-op
	}
}