#include "wadarchive.hpp"
#include "wadentry.hpp"

#include "utils.hpp"

using namespace std;

namespace wadarchive
{
	/// @brief Create a wad writer with a specific file
	/// @param destination_file The location of the file
	WadArchiveWriter::WadArchiveWriter(string destination_file)
	{
		file_location = destination_file;

		// Check if the file exists
		if (utils::file_exists(file_location)) {
			// delete the file
			remove(file_location);
		}

		// Create an empty file
		
	}

	/// @brief Destroy the writer
	WadArchiveWriter::~WadArchiveWriter() {}

	/// @brief Wrap up the file
	void close()
	{
		// no op
	}

	/// @brief Add a new file entry to the wad
	/// @param filename The name of the entry
	/// @param data The data to be written
	WadEntry *AddFile(string filename, char *data)
	{
	}
}