#pragma once

#include <fstream>
#include <stdlib.h>
#include <vector>
#include <filesystem>

#include "wadentry.hpp"

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
		vector<string> ls_recursive(string path);

		/// @brief Get the size of a file
		/// @param filename The path to the file
		/// @return The file size in bytes
		int filesize(string filename);

		/// @brief Get an integer as a string
		/// @param num The number to format
		/// @return A nicely formatted string
		string get_int_string(int num);

		/// @brief Read all bytes from a file
		/// @param filename The file to read
		/// @param read How many bytes were read
		/// @return The data
		char *read_file(char *filename, int *read);

		/// @brief Read a range of bytes from a file
		/// @param filename The file to read
		/// @param start The byte position to start at
		/// @param stride How many bytes to read
		/// @return The data
		char *read_file_range(char *filename, int start, int stride);

		/// @brief Write a fixed length string using a shorter input string
		/// @param srcstr The shorter input string
		/// @param length How long to make the fixed length string
		/// @param fillchar What character to fill the remainder of the string with
		/// @return The resulting fixed length string
		char *write_fixed_length_string(string srcstr, int length, char fillchar);

		/// @brief Combine two path strings into one
		/// @param p1 First path
		/// @param p2 Second path
		/// @return Final combined path
		string path_join(string p1, string p2);

		/// @brief Ensure a relative path exists
		/// @param rootpath The root path that exists
		/// @param relpath The new relative path
		void relative_path_create(string rootpath, string relpath);

		/// @brief Read a range of bytes
		/// @param filename The filename
		/// @param start Starting point
		/// @param stride The length
		/// @return A vector of bytes
		vector<char> read_file_range_as_vector(char *filename, int start, int stride);
	}
}