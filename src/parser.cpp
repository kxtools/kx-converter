#include "kx_converter/parser.h"
#include <vector>
#include <string>
#include <algorithm>

namespace kx_converter
{
	/**
	 * @brief Checks if a string can be safely parsed as a double.
	 * This is a robust check used to prevent std::stod from throwing exceptions.
	 * @param s The string to check.
	 * @return True if the string is a valid number, false otherwise.
	 */
	static bool is_numeric(const std::string& s)
	{
		if (s.empty())
		{
			return false;
		}
		char* end = nullptr;
		strtod(s.c_str(), &end);
		// A successful conversion means the end pointer is not where it started,
		// and it points to the null terminator at the end of the string.
		return end != s.c_str() && *end == '\0';
	}

	std::optional<Models::Coordinates> LineParser::parse(const std::string& line) const
	{
		std::string processed_line = line;

		// Pre-processing: Replace commas with spaces to handle comma-separated coordinates.
		std::replace(processed_line.begin(), processed_line.end(), ',', ' ');

		// 1. Tokenize the entire line by any whitespace.
		// This is the first step in breaking down the line into manageable parts.
		std::vector<std::string> parts;
		std::string current_part;
		std::istringstream stream(processed_line);
		while (stream >> current_part)
		{
			parts.push_back(current_part);
		}

		// 2. A valid line must have at least 4 parts (e.g., "1 'n' X Y Z").
		if (parts.size() < 4)
		{
			return std::nullopt;
		}

		// 3. Iterate BACKWARDS from the end of the tokens to find the first
		//    sequence of three consecutive numeric tokens. This is the most robust
		//    way to find coordinates, as it's immune to malformed names.
		for (int i = parts.size() - 3; i >= 1; --i)
		{
			const std::string& x_str = parts[i];
			const std::string& y_str = parts[i + 1];
			const std::string& z_str = parts[i + 2];

			if (is_numeric(x_str) && is_numeric(y_str) && is_numeric(z_str))
			{
				// SUCCESS: We found the coordinate block.

				// 4. Assemble the name from all tokens between the first (number)
				//    and the start of the coordinate block.
				std::string name;
				for (size_t j = 1; j < i; ++j)
				{
					name += parts[j] + " ";
				}
				if (!name.empty())
				{
					name.pop_back(); // Remove the final trailing space.
				}

				// Clean up surrounding quotes from the assembled name.
				if (name.length() >= 2 && name.front() == '\'' && name.back() == '\'')
				{
					name = name.substr(1, name.length() - 2);
				}

				// 5. Convert the verified numeric strings to doubles.
				// This is now guaranteed to be safe from `bad_conversion` exceptions.
				double x = std::stod(x_str);
				double y = std::stod(y_str);
				double z = std::stod(z_str);

				// Return the final struct, with Y and Z swapped as required.
				return Models::Coordinates{name, x, z, y};
			}
		}

		// If the loop completes without finding a valid coordinate block, the line is invalid.
		return std::nullopt;
	}
} // namespace kx_converter
