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
		format_version = ENGINE_VERSION;

		// Check if the file exists
		if (utils::file_exists(file_location))
		{
			// delete the file
			remove(file_location.c_str());
		}
		std::ofstream file(file_location, std::ios::binary);

		if (file.is_open())
		{
			// Write the character array to the file
			char *sigbuffer = utils::write_fixed_length_string(ENGINE_NAME, FRONT_BUFFER_LENGTH, ' ');
			file.write(sigbuffer, FRONT_BUFFER_LENGTH);
			free(sigbuffer);

			char *sigverbuffer = utils::write_fixed_length_string(utils::get_double_as_string(ENGINE_VERSION, 2), FRONT_BUFFER_LENGTH, ' ');
			file.write(sigverbuffer, FRONT_BUFFER_LENGTH);
			free(sigverbuffer);

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
		format_version = ENGINE_VERSION;
	}

	/// @brief Initialize the reader with the file
	/// @return Whether or not it was successful
	bool WadArchiveReader::init()
	{
		// Get file length
		int file_length = utils::filesize(file_location);

		// Read the file signature
		char *sigstrchar = utils::read_file_range((char *)file_location.c_str(), 0, FRONT_BUFFER_LENGTH);
		string sigstrc = string(sigstrchar);
		free(sigstrchar);

		if (!sigstrc.starts_with(ENGINE_NAME))
		{
			// Fails sig check
			return false;
		}

		char *verstrchar = utils::read_file_range((char *)file_location.c_str(), FRONT_BUFFER_LENGTH, FRONT_BUFFER_LENGTH);
		string verstrc = string(verstrchar);
		free(verstrchar);
		format_version = stof(verstrc);

		if (format_version < MIN_READABLE_ENGINE_VERSION)
		{
			// Fails sig check
			return false;
		}

		// Read the footer and JSON data
		char *jsonposstrchar = utils::read_file_range((char *)file_location.c_str(), file_length - SUFFIX_BUFFER_LENGTH, SUFFIX_BUFFER_LENGTH);
		int jsonpos = stoi(string(jsonposstrchar));
		free(jsonposstrchar);
		char *jsonchardata = utils::read_file_range((char *)file_location.c_str(), jsonpos, file_length - SUFFIX_BUFFER_LENGTH - jsonpos);
		json filedata = json::parse(jsonchardata);
		free(jsonchardata);

		// iterate over the entries
		if (filedata["engine"] != ENGINE_NAME)
		{
			return false;
		}
		if (filedata["engine_ver"] < MIN_READABLE_ENGINE_VERSION)
		{
			return false;
		}
		json entrydata = filedata["entries"];
		for (auto &el : entrydata.items())
		{
			json entrydata = el.value();
			string ename = entrydata["name"];
			int esize = entrydata["size"];
			int eloc = entrydata["location"];
			WadEntry *wad = new WadEntry(ename, NULL, esize);
			wad->byte_location = eloc;
			entry_info.push_back(wad);
		}
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