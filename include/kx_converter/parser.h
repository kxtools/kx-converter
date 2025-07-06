#pragma once

#include "kx_converter/models.h"
#include <optional>
#include <string>
#include <sstream>

/**
 * @namespace kx_converter
 * @brief Main namespace for the kx-converter library.
 */
namespace kx_converter
{
	/**
	 * @class LineParser
	 * @brief Responsible for parsing single lines from KX-Map format files.
	 *
	 * This parser is designed to be highly robust against the messy and
	 * inconsistent formatting found in user-generated text files.
	 */
	class LineParser
	{
	public:
		/**
		 * @brief Parses a single line from a kx-map file.
		 *
		 * This function identifies a checkpoint number, a name, and three coordinate values.
		 * It is robust against variations in whitespace, quoting, and special characters
		 * in the name. A key feature is swapping the Y and Z coordinates upon success.
		 *
		 * @param line The string content of the line to parse.
		 * @return An std::optional containing a Coordinates struct on successful parsing.
		 *         Returns std::nullopt if the line is malformed or cannot be parsed.
		 */
		std::optional<Models::Coordinates> parse(const std::string& line) const;
	};
} // namespace kx_converter
