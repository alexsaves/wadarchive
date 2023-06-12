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
	}
}

#endif