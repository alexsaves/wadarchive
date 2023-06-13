#include <iostream>
#include <stdlib.h>
#include <filesystem>

#include <args.h>

#include "../include/utils.hpp"
#include "../include/constants.hpp"

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
	parser.helptext = "Usage: wadarchive -s test/testdata -d out.wad";
	parser.version = utils::get_double_as_string(ENGINE_VERSION, 2);
	parser.option("dest d", "archive.wad");
	parser.option("src s");
	parser.parse(argc, argv);

	string source_glob = parser.value("s");
	string dest_wad = parser.value("d");

	// See if things are obviously invalid
	if (dest_wad.length() == 0 || source_glob.length() == 0)
	{
		cout << parser.helptext << endl;
		return EXIT_SUCCESS;
	}

	// Check if its a simple folder input
	bool source_is_dir = utils::location_is_folder((char *)source_glob.c_str());

	if (!source_is_dir) {
		cout << "Error: Source location must be a folder (eg \"files/files_to_zip\")." << endl;
		return EXIT_SUCCESS;
	}

	cout << "Looking in \"" << source_glob << "\"" << endl;

	const std::filesystem::path path(source_glob);

	cout << "parent_path: " << path.parent_path() << endl;
	cout << "stem: " << path.stem() << endl;
	cout << "root_path: " << path.root_path() << endl;
	cout << "root_directory: " << path.root_directory() << endl;

	vector<string> file_list = utils::ls_recursive(source_glob);

	unsigned int match_count = file_list.size();

	for (unsigned int i = 0; i < match_count; i++)
	{
		string final_file = file_list[i].replace(0, source_glob.length(), "");
		cout << "FILE: " << file_list[i] << " becomes: " << final_file << endl;
	}
	/*vector<path> file_list = glob::rglob(source_glob);
	unsigned int match_count = file_list.size();

	for (unsigned int i = 0; i < match_count; i++)
	{
		if (utils::location_is_file((char *)file_list[i].c_str()))
		{
			cout << "New file------" << endl;
			path fpath = file_list[i];			
			cout << fpath.relative_path() << endl;
			cout << fpath.root_directory() << endl;
			cout << fpath.root_path() << endl;
		}
	}*/

	return EXIT_SUCCESS;
}