#include <iostream>
#include <stdlib.h>
#include <filesystem>

#include <args.h>

#include "../include/utils.hpp"
#include "../include/constants.hpp"
#include "../include/wadarchive.hpp"

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
	cout << ENGINE_NAME << " v" << utils::get_double_as_string(ENGINE_VERSION, 2) << endl;

	args::ArgParser parser;
	parser.helptext = "Usage to archive a folder: wadarchive -a -s test/testdata -d archive.wad\nUsage to extract a wadfile: wadarchive -x -s archive.wad -d ./";
	parser.version = utils::get_double_as_string(ENGINE_VERSION, 2);
	parser.option("dest d", "archive.wad");
	parser.option("src s");
	parser.flag("archive a");
	parser.flag("extract x");
	parser.flag("quiet q");
	parser.parse(argc, argv);

	string source_glob = parser.value("s");
	string dest_wad = parser.value("d");

	bool is_quiet = parser.found("q");

	// See if things are obviously invalid
	if (dest_wad.length() == 0 || source_glob.length() == 0)
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

	if (archive_mode == archive)
	{
		if (!is_quiet)
			cout << "Wadding folder " << source_glob << "..." << endl;

		// Check if its a simple folder input
		bool source_is_dir = utils::location_is_folder((char *)source_glob.c_str());

		if (!source_is_dir)
		{
			cout << "Error: Source location must be a folder (eg \"files/files_to_zip\")." << endl;
			return EXIT_SUCCESS;
		}

		WadArchiveWriter writer(dest_wad);

		vector<string> file_list = utils::ls_recursive(source_glob);
		unsigned int match_count = file_list.size();
		for (unsigned int i = 0; i < match_count; i++)
		{
			string full_file_path = string(file_list[i]);
			char * buf = utils::read_file(full_file_path);
			string final_dest_file = full_file_path.replace(0, source_glob.length(), "");
			if (final_dest_file[0] == '/' || final_dest_file[0] == '\\')
			{
				final_dest_file = final_dest_file.erase(0, 1);
			}
			writer.AddFile(final_dest_file, buf);
			free(buf);

			if (!is_quiet)
				cout << final_dest_file << "." << endl;
		}

		writer.close();

		return EXIT_SUCCESS;
	}
	else
	{
		if (!utils::file_exists(source_glob))
		{
			cout << "Error: WAD not found: " << source_glob << endl;
			return EXIT_SUCCESS;
		}

		if (!is_quiet)
			cout << "Unwadding " << source_glob << "..." << endl;
	}

	return EXIT_SUCCESS;
}