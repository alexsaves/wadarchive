#include <iostream>
#include <stdlib.h>

#include <cargs.h>

#include "../include/utils.hpp"
#include "../include/constants.hpp"

using namespace std;
using namespace wadarchive;

/**
 * This is the main configuration of all options available.
 */
static struct cag_option options[] = {
	{.identifier = 's',
	 .access_letters = "s",
	 .access_name = NULL,
	 .value_name = NULL,
	 .description = "Source files or folder"},

	{.identifier = 'd',
	 .access_letters = "d",
	 .access_name = NULL,
	 .value_name = NULL,
	 .description = "Target wadfile to make"},

	{.identifier = 'h',
	 .access_letters = "h",
	 .access_name = "help",
	 .description = "Shows the command help"}};

/**
 * This is a custom project configuration structure where you can store the
 * parsed information.
 */
struct wad_config
{
	bool has_source;
	bool has_destination;
	const char *source_dir;
	const char *dest_wad;
};

/// @brief Print the help text
/// @param opts 
void print_help()
{
	printf("Usage: wadarchive [OPTION]...\n");
	cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
	printf("\nExample: wadarchive -s=files/testdata -d=out.wad\n");
}

/// @brief Archiver CLI entrypoint
/// @param argc Number of command line arguments
/// @param argv Command line arguments
/// @return Error/Success code
int main(int argc, char *argv[])
{
	cout << ENGINE_NAME << " v" << utils::get_double_as_string(ENGINE_VERSION, 2) << endl;

	char identifier;
	cag_option_context context;
	struct wad_config config = {false, false, NULL, NULL};

	/**
	 * Now we just prepare the context and iterate over all options. Simple!
	 */
	cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
	while (cag_option_fetch(&context))
	{
		identifier = cag_option_get(&context);
		switch (identifier)
		{
		case 's':
			config.has_source = true;
			config.source_dir = cag_option_get_value(&context);
			break;
		case 'd':
			config.has_destination = true;
			config.dest_wad = cag_option_get_value(&context);
			break;
		case 'h':
			print_help();
			return EXIT_SUCCESS;
		}
	}

	if (!config.has_source || !config.has_destination)
	{
		print_help();
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}