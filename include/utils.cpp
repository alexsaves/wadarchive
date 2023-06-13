#include "utils.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>

namespace wadarchive
{
	namespace utils
	{
		/// @brief Check if a file exists
		/// @param name Location of the file
		/// @return Whether or not the file exists
		bool file_exists(const string &name)
		{
			return (access(name.c_str(), F_OK) != -1);
		}

		/// @brief Get a floating point number as a string
		/// @param num The number to format
		/// @param precision The number of decimals
		/// @return A nicely formatted string
		string get_double_as_string(double num, int precision)
		{
			stringstream stream;
			stream << fixed << setprecision(precision) << num;
			return stream.str();
		}
	}
}