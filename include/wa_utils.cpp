#include "wa_utils.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <iostream>
#include <vector>

#include "wadentry.hpp"

namespace fs = std::filesystem;

namespace wadarchive
{
	namespace wautils
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
		vector<string> ls_recursive(string path)
		{
			vector<string> files;
			filesystem::path npath(path);
			for (const auto &p : filesystem::recursive_directory_iterator(npath))
			{
				if (!std::filesystem::is_directory(p))
				{
					files.push_back(p.path().generic_string());
				}
			}
			return files;
		}

		/// @brief Read a file as a character array
		/// @param path The path to the file
		/// @return The character array of the file
		WadEntry *read_file(string path)
		{
			std::ifstream file(path, std::ios::binary); // Open the binary file

			// Get the size of the file
			file.seekg(0, std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			// Read the file into a vector<char>
			std::vector<char> data(fileSize);
			file.read(data.data(), fileSize);

			// Close the file
			file.close();

			WadEntry *wentry = new WadEntry(path, data);
			return wentry;
		}

		/// @brief Get the size of a file
		/// @param filename The path to the file
		/// @return The file size in bytes
		int filesize(string filename)
		{
			std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
			return (int)in.tellg();
		}

		/// @brief Read all bytes from a file
		/// @param filename The file to read
		/// @param read How many bytes were read
		/// @return The data
		char *read_file(char *filename, int *read)
		{
			ifstream ifs(filename, ios::binary | ios::ate);
			ifstream::pos_type pos = ifs.tellg();
			int length = (int)pos;
			char *pChars = new char[length + 1];
			ifs.seekg(0, ios::beg);
			ifs.read(pChars, length);
			ifs.close();
			pChars[length] = '\0';
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
			char *pChars = new char[stride + 1];
			ifs.seekg(start);
			ifs.read(pChars, stride);
			ifs.close();
			// Add null termination
			pChars[stride] = '\0';
			return pChars;
		}

		/// @brief Write a fixed length string using a shorter input string
		/// @param srcstr The shorter input string
		/// @param length How long to make the fixed length string
		/// @param fillchar What character to fill the remainder of the string with
		/// @return The resulting fixed length string
		char *write_fixed_length_string(string srcstr, int length, char fillchar)
		{
			char *buffer = new char[length];
			std::fill_n(buffer, length, fillchar);

			std::copy(srcstr.begin(), srcstr.end(), buffer);
			return buffer;
		}

		/// @brief Combine two path strings into one
		/// @param p1 First path
		/// @param p2 Second path
		/// @return Final combined path
		string path_join(string p1, string p2)
		{
			char sep = '/';
			string tmp = p1;

#ifdef _WIN32
			sep = '\\';
#endif

			// Add separator if it is not included in the first path:
			if (p1[p1.length() - 1] != sep)
			{
				tmp += sep;
				return tmp + p2;
			}
			else
			{
				return p1 + p2;
			}
		}

		/// @brief Ensure a relative path exists
		/// @param rootpath The root path that exists
		/// @param relpath The new relative path
		void relative_path_create(string rootpath, string relpath)
		{
			char splitchar = '/';
			std::size_t found = relpath.find("\\");
			if (found != std::string::npos)
				splitchar = '\\';
			std::stringstream test(relpath);
			std::string segment;
			std::vector<std::string> seglist;
			string final_path_str = rootpath;
			while (std::getline(test, segment, splitchar))
			{
				if (segment.length() > 0)
				{
					final_path_str = path_join(final_path_str, segment);
					if (!filesystem::is_directory(final_path_str) || !filesystem::exists(final_path_str))
					{
						filesystem::create_directory(final_path_str);
					}
				}
			}
		}

		/// @brief Read a range of bytes
		/// @param filename The filename
		/// @param start Starting point
		/// @param stride The length
		/// @return A vector of bytes
		vector<char> read_file_range_as_vector(char *filename, int start, int stride)
		{
			std::ifstream file(filename, std::ios::binary); // Open the file in binary mode

			// Get the size of the file
			file.seekg(0, std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			if (start + stride > fileSize)
			{
				stride = (int)fileSize - (int)start;
			}

			// Move the file pointer to the start position
			file.seekg(start, std::ios::beg);

			// Read the specified range of bytes into a vector<char>
			std::vector<char> data(stride);
			file.read(data.data(), stride);

			// Close the file
			file.close();
			return data;
		}
	}
}