#ifndef WARCHIVE_WARCHIVE_H
#define WARCHIVE_WARCHIVE_H
#include <stdlib.h>
#include <iostream>

#include "wadentry.hpp"

using namespace std;

namespace wadarchive
{
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
		WadEntry * AddFile(string filename, char *data, int datalen);

		/// @brief Wrap up the file
		void close();

		/// @brief The location of the file being written
		string file_location;

		/// @brief Holds the list of entries
		vector<WadEntry> entries;
    };
}

#endif
