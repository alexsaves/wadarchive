#ifndef WARCHIVE_UTILS_INCLUDE
#define WARCHIVE_UTILS_INCLUDE

#include <fstream>
#include <stdlib.h>

using namespace std;

/**
 * Namespace containing utility functions
 */
namespace wadarchive
{
	namespace utils
	{
		/// @brief Check if a file exists
		/// @param name Location of the file
		/// @return Whether or not the file exists
		bool file_exists(const std::string &name);

		/// @brief Get a floating point number as a string
		/// @param num The number to format
		/// @param precision The number of decimals
		/// @return A nicely formatted string
		string get_double_as_string(double num, int precision);

		/// @brief See if the location points directly to a file
		/// @param src The file location
		/// @return Whether or not its a file
		bool location_is_file(char *src);

		/// @brief See if the location points directly to a folder
		/// @param src The file or folder location
		/// @return Whether or not its a folder
		bool location_is_folder(char *src);

		/// @brief Get a recursive list of all files in a folder
		/// @param path The starting folder
		/// @return The list of files
		vector<string> ls_recursive(const std::filesystem::path &path);
	}
}

#endif