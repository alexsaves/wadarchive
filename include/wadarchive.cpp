#include "wadarchive.hpp"
#include "wadentry.hpp"

#include "wa_utils.hpp"
#include "wa_constants.hpp"

#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <map>
#include <filesystem>

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
		format_version = WA_ENGINE_VERSION;

		// Check if the file exists
		if (wautils::file_exists(file_location))
		{
			// delete the file
			remove(file_location.c_str());
		}
		std::ofstream file(file_location, std::ios::binary);

		if (file.is_open())
		{
			// Write the character array to the file
			char *sigbuffer = wautils::write_fixed_length_string(WA_ENGINE_NAME, FRONT_BUFFER_LENGTH, ' ');
			file.write(sigbuffer, FRONT_BUFFER_LENGTH);
			free(sigbuffer);

			char *sigverbuffer = wautils::write_fixed_length_string(wautils::get_double_as_string(WA_ENGINE_VERSION, 2), FRONT_BUFFER_LENGTH, ' ');
			file.write(sigverbuffer, FRONT_BUFFER_LENGTH);
			free(sigverbuffer);

			// Close the file
			file.close();
		}
	}

	/// @brief Destroy the writer
	WadArchiveWriter::~WadArchiveWriter()
	{
		unsigned int entrycount = (unsigned int)entries.size();
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
		int jsonlocation = wautils::filesize(file_location);

		json j;

		j["engine"] = WA_ENGINE_NAME;
		j["engine_ver"] = WA_ENGINE_VERSION;

		json entrylist;

		unsigned int entrycount = (unsigned int)entries.size();
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
		//char buffer[s.length()];
		char* buffer = new char[s.size()];
		std::copy(s.begin(), s.end(), buffer);

		// Dump the json location to a buffer
		string jsonlocationstr = wautils::get_int_string(jsonlocation);
		const int locbuffersize = SUFFIX_BUFFER_LENGTH;
		char* locbuffer = new char[locbuffersize];
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
		delete[] buffer;
		delete[] locbuffer;
	}

	/// @brief Add a new file entry to the wad
	/// @param filename The name of the entry
	/// @param data The data to be written
	WadEntry *WadArchiveWriter::AddFile(string filename, vector<char> data)
	{
		WadEntry *wad = new WadEntry(filename, data);
		wad->byte_location = wautils::filesize(file_location);

		std::ofstream file(file_location, std::ios::binary | std::ios::app);

		if (file.is_open())
		{
			// Write the character array to the file
			file.write(data.data(), data.size());

			// Close the file
			file.close();
		}
		entries.push_back(wad);
		wad->free_just_memory();
		return wad;
	}

	/// @brief Reads a wad archive from the file system
	/// @param source_wad_file The location of the file
	WadArchiveReader::WadArchiveReader(string source_wad_file)
	{
		file_location = source_wad_file;
		format_version = WA_ENGINE_VERSION;
	}

	/// @brief Initialize the reader with the file
	/// @return Whether or not it was successful
	bool WadArchiveReader::init()
	{
		// Get file length
		int file_length = wautils::filesize(file_location);

		// Read the file signature
		char *sigstrchar = wautils::read_file_range((char *)file_location.c_str(), 0, FRONT_BUFFER_LENGTH);
		string sigstrc = string(sigstrchar);
		free(sigstrchar);

		if (!sigstrc.starts_with(WA_ENGINE_NAME))
		{
			// Fails sig check
			return false;
		}

		char *verstrchar = wautils::read_file_range((char *)file_location.c_str(), FRONT_BUFFER_LENGTH, FRONT_BUFFER_LENGTH);
		string verstrc = string(verstrchar);
		free(verstrchar);
		format_version = stof(verstrc);

		if (format_version < MIN_READABLE_ENGINE_VERSION)
		{
			// Fails sig check
			return false;
		}

		// Read the footer and JSON data
		char *jsonposstrchar = wautils::read_file_range((char *)file_location.c_str(), file_length - SUFFIX_BUFFER_LENGTH, SUFFIX_BUFFER_LENGTH);
		int jsonpos = stoi(string(jsonposstrchar));
		free(jsonposstrchar);
		char *jsonchardata = wautils::read_file_range((char *)file_location.c_str(), jsonpos, file_length - SUFFIX_BUFFER_LENGTH - jsonpos);
		json filedata = json::parse(jsonchardata);
		free(jsonchardata);

		// iterate over the entries
		if (filedata["engine"] != WA_ENGINE_NAME)
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
			int eloc = entrydata["location"];
			WadEntry *wad = new WadEntry();
			wad->file_name = ename;
			wad->size = (size_t)entrydata["size"];
			wad->byte_location = eloc;
			entry_info.push_back(wad);
			entry_names.insert(pair<string, WadEntry *>(ename, wad));
		}
		return true;
	}

	/// @brief Is the provided file present?
	/// @param filename The file name
	/// @return Whether the file is present
	bool WadArchiveReader::has_file(string filename)
	{
		return entry_names.contains(filename);
	}

	/// @brief Get the entries in the archive
	/// @return The list of entries by filename
	vector<string> WadArchiveReader::get_entries()
	{
		vector<string> out_entries;
		unsigned int entrycount = (unsigned int)entry_info.size();
		for (unsigned int i = 0; i < entrycount; i++)
		{
			WadEntry *wad = entry_info[i];
			out_entries.push_back(wad->file_name);
		}
		return out_entries;
	}

	/// @brief Get the file as a wadentry. This will return a fresh copy and not retain the data itself.
	/// @param filename The filename to get
	/// @return The wad entry
	WadEntry *WadArchiveReader::get_file(string filename)
	{
		WadEntry *entry = entry_names[filename];
		WadEntry *newEntry = new WadEntry();
		newEntry->byte_location = entry->byte_location;
		newEntry->file_name = entry->file_name;
		newEntry->size = entry->size;
    
		vector<char> filedata = wautils::read_file_range_as_vector((char *)file_location.c_str(), newEntry->byte_location, (int)newEntry->size);
		newEntry->setData(filedata);

		return newEntry;
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