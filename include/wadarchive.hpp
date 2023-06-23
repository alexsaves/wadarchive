#pragma once

#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <filesystem>

#include "wadentry.hpp"

using namespace std;

namespace wadarchive
{
	/// @brief Writes a wad archive
	class WadArchiveWriter
	{
	public:
		/// @brief Create a wad writer with a specific file
		/// @param destination_file The location of the file
		WadArchiveWriter(string destination_file);

		/// @brief Destroy the writer
		~WadArchiveWriter();

		/// @brief Add a new file entry to the wad
		/// @param filename The name of the entry
		/// @param data The data to be written
		WadEntry *AddFile(string filename, vector<char> data);

		/// @brief Wrap up the file
		void close();

		/// @brief The location of the file being written
		string file_location;

		/// @brief The version of the file format
		float format_version;

		/// @brief Holds the list of entries
		vector<WadEntry *> entries;
	};

	/// @brief Reads a wad archive
	class WadArchiveReader
	{
	public:
		/// @brief Reads a wad archive from the file system
		/// @param source_wad_file The location of the file
		WadArchiveReader(string source_wad_file);

		/// @brief Destroy the reader
		~WadArchiveReader();

		/// @brief Wrap up the reader
		void close();

		/// @brief Initialize the reader with the file
		/// @return Whether or not it was successful
		bool init();

		/// @brief Is the provided file present?
		/// @param filename The file name
		/// @return Whether the file is present
		bool has_file(string filename);

		/// @brief Get the file as a wadentry. This will return a fresh copy and not retain the data itself.
		/// @param filename The filename to get
		/// @return The wad entry
		WadEntry *get_file(string filename);

		/// @brief Get the entries in the archive
		/// @return The list of entries by filename
		vector<string> get_entries();

		/// @brief The location of the file being read
		string file_location;

		/// @brief The version of the file format
		float format_version;

	private:
		/// @brief Holds the entry names
		vector<WadEntry *> entry_info;

		/// @brief A convenient holder of all the entry names
		map<string, WadEntry *> entry_names;
	};
}