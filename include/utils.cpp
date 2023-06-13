#include "utils.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

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

		/// @brief See if the location points directly to a file
		/// @param src The file location
		/// @return Whether or not its a file
		bool location_is_file(char *src)
		{
			const fs::path path(src);
			std::error_code ec;
			if (fs::is_directory(path, ec))
			{
				// Process a directory.
				return false;
			}
			if (fs::is_regular_file(path, ec))
			{
				// Process a regular file.
				return true;
			}
			return false;
		}

		/// @brief See if the location points directly to a folder
		/// @param src The file or folder location
		/// @return Whether or not its a folder
		bool location_is_folder(char *src)
		{
			const fs::path path(src);
			std::error_code ec;
			if (fs::is_directory(path, ec))
			{
				// Process a directory.
				return true;
			}
			if (fs::is_regular_file(path, ec))
			{
				// Process a regular file.
				return false;
			}
			return false;
		}

		/// @brief Get a recursive list of all files in a folder
		/// @param path The starting folder
		/// @return The list of files
		vector<string> ls_recursive(const std::filesystem::path &path)
		{
			vector<string> files;
			for (const auto &p : std::filesystem::recursive_directory_iterator(path))
			{
				if (!std::filesystem::is_directory(p))
				{
					files.push_back(p.path());
				}
			}
			return files;
		}

		/// @brief Read a file as a character array
		/// @param path The path to the file
		/// @return The character array of the file
		char *read_file(string path)
		{
			char *buffer;
			long size;
			ifstream file(path, ios::in | ios::binary | ios::ate);
			size = file.tellg();
			file.seekg(0, ios::beg);
			buffer = new char[size];
			file.read(buffer, size);
			file.close();
			return buffer;
		}
	}
}