#include <iostream>
#include <stdlib.h>
#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>

#include <args.h>

#include "../include/wa_utils.hpp"
#include "../include/wa_constants.hpp"
#include "../include/wadarchive.hpp"
#include "../include/wadentry.hpp"

using namespace std;
using namespace filesystem;
using namespace wadarchive;
using namespace args;

/// @brief Archiver CLI entrypoint
/// @param argc Number of command line arguments
/// @param argv Command line arguments
/// @return Error/Success code
int main(int argc, char *argv[])
{
	// Set up output boilerplate
	cout << wadarchive::ENGINE_NAME << " v" << wautils::get_double_as_string(wadarchive::ENGINE_VERSION, 2) << endl;

	args::ArgParser parser;
	parser.helptext = "Usage to archive a folder: wadarchive -a -s test/testdata -d archive.wad\nUsage to extract a wadfile: wadarchive -x -s archive.wad -d ./";
	parser.version = wautils::get_double_as_string(wadarchive::ENGINE_VERSION, 2);
	parser.option("dest d", "archive.wad");
	parser.option("src s");
	parser.flag("archive a");
	parser.flag("extract x");
	parser.flag("quiet q");
	parser.parse(argc, argv);

	string source_path_str = parser.value("s");
	string dest_path_str = parser.value("d");

	bool is_quiet = parser.found("q");

	// See if things are obviously invalid
	if (dest_path_str.length() == 0 || source_path_str.length() == 0)
	{
		cout << parser.helptext << endl;
		return EXIT_SUCCESS;
	}

	enum ARCHIVE_MODE
	{
		archive,
		extract
	};
	ARCHIVE_MODE archive_mode = archive;

	if (!parser.found("a") && !parser.found("x"))
	{
		cout << "Error: You must specify whether you are archiving or extracting." << endl
			 << parser.helptext << endl;
		return EXIT_SUCCESS;
	}

	if (parser.found("x"))
	{
		// extract mode
		archive_mode = extract;
	}

	if (archive_mode == archive)
	{
		if (!is_quiet)
			cout << "Wadding folder " << source_path_str << "..." << endl;

		// Check if its a simple folder input
		bool source_is_dir = wautils::location_is_folder((char *)source_path_str.c_str());

		if (!source_is_dir)
		{
			cout << "Error: Source location must be a folder (eg \"files/files_to_zip\")." << endl;
			return EXIT_SUCCESS;
		}

		WadArchiveWriter writer(dest_path_str);

		vector<string> file_list = wautils::ls_recursive(source_path_str);
		unsigned int match_count = file_list.size();
		for (unsigned int i = 0; i < match_count; i++)
		{
			string full_file_path = string(file_list[i]);
			WadEntry *entry = wautils::read_file(full_file_path);
			string final_dest_file = full_file_path.replace(0, source_path_str.length(), "");
			if (final_dest_file[0] == '/' || final_dest_file[0] == '\\')
			{
				final_dest_file = final_dest_file.erase(0, 1);
			}
			writer.AddFile(final_dest_file, entry->getData());

			delete entry;

			if (!is_quiet)
				cout << final_dest_file << "." << endl;
		}

		writer.close();

		return EXIT_SUCCESS;
	}
	else
	{
		if (!wautils::file_exists(source_path_str))
		{
			cout << "Error: WAD not found: " << source_path_str << endl;
			return EXIT_SUCCESS;
		}

		if (wautils::location_is_file((char *)dest_path_str.c_str()))
		{
			cout << "Error: Destination is a file, not a path: " << dest_path_str << endl;
			return EXIT_SUCCESS;
		}

		if (!is_quiet)
			cout << "Unwadding " << source_path_str << "..." << endl;

		WadArchiveReader reader(source_path_str);

		if (!reader.init())
		{
			cout << "Error: not a valid archive." << endl;
			return EXIT_SUCCESS;
		}

		if (reader.get_entries().size() == 0)
		{
			cout << "Error: no files found in the wad." << endl;
			return EXIT_SUCCESS;
		}

		vector<string> file_entries = reader.get_entries();

		filesystem::path dest_path(dest_path_str);
		int totalfilesize = 0;

		for (int i = 0; i < (int)file_entries.size(); i++)
		{
			WadEntry *wad = reader.get_file(file_entries[i]);
			totalfilesize += wad->size;
			filesystem::path path(file_entries[i]);
			string final_path_str = wautils::path_join(dest_path_str, path.parent_path().generic_string());
			if (!filesystem::is_directory(final_path_str) || !filesystem::exists(final_path_str))
			{
				wautils::relative_path_create(dest_path_str, path.parent_path().generic_string());
			}

			// Write the file
			string final_file_path_str = wautils::path_join(dest_path_str, file_entries[i]);
			std::ofstream file(final_file_path_str, std::ios::binary);

			if (file.is_open())
			{
				file.write(wad->getData().data(), wad->size);

				// Close the file
				file.close();
			}

			if (!is_quiet)
			{
				if (i == 0)
				{
					cout << endl;
				}
				cout << file_entries[i] << " (" << wad->size << " bytes)" << endl;
			}
			delete (wad);
		}

		reader.close();
		if (!is_quiet)
		{
			cout << endl
				 << "Unwadded " << file_entries.size() << " files (" << totalfilesize << " bytes)." << endl;
		}
	}

	return EXIT_SUCCESS;
}