#include "utils.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <iostream>

#include "wadentry.hpp"

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

		/// @brief Get an integer as a string
		/// @param num The number to format
		/// @return A nicely formatted string
		string get_int_string(int num)
		{
			stringstream stream;
			stream << num;
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
		WadEntry *read_file(string path)
		{
			char *buffer;
			long size;
			ifstream file(path, ios::in | ios::binary | ios::ate);
			size = file.tellg();
			file.seekg(0, ios::beg);
			buffer = new char[size];
			file.read(buffer, size);
			file.close();
			WadEntry *wentry = new WadEntry(path, buffer, size);
			return wentry;
		}

		/// @brief Get the size of a file
		/// @param filename The path to the file
		/// @return The file size in bytes
		int filesize(string filename)
		{
			std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
			return in.tellg();
		}

		/// @brief Read all bytes from a file
		/// @param filename The file to read
		/// @param read How many bytes were read
		/// @return The data
		char *read_file(char *filename, int *read)
		{
			ifstream ifs(filename, ios::binary | ios::ate);
			ifstream::pos_type pos = ifs.tellg();
			int length = pos;
			char *pChars = new char[length];
			ifs.seekg(0, ios::beg);
			ifs.read(pChars, length);
			ifs.close();
			*read = length;
			return pChars;
		}

		/// @brief Read a range of bytes from a file
		/// @param filename The file to read
		/// @param start The byte position to start at
		/// @param stride How many bytes to read
		/// @return The data
		char *read_file_range(char *filename, int start, int stride)
		{
			ifstream ifs(filename, ios::binary | ios::ate);
			char *pChars = new char[stride];
			ifs.seekg(start);
			ifs.read(pChars, stride);
			ifs.close();
			return pChars;
		}

		/// @brief Write a fixed length string using a shorter input string
		/// @param srcstr The shorter input string
		/// @param length How long to make the fixed length string
		/// @param fillchar What character to fill the remainder of the string with
		/// @return The resulting fixed length string
		char *write_fixed_length_string(string srcstr, int length, char fillchar) {
			char *buffer = new char[length];
			std::fill_n(buffer, length, fillchar);

			std::copy(srcstr.begin(), srcstr.end(), buffer);
			return buffer;
		}
	}
}