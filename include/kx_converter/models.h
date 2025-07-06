#pragma once

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

/**
 * @namespace kx_converter::Models
 * @brief Contains the data structures used to represent map data.
 */
namespace kx_converter::Models
{
	/**
	 * @struct Coordinates
	 * @brief Represents a single named 3D coordinate point.
	 */
	struct Coordinates
	{
		std::string name; //!< The name of the checkpoint or point of interest.
		double x{0.0}; //!< The X coordinate.
		double y{0.0}; //!< The Y coordinate (swapped from the source file's Z).
		double z{0.0}; //!< The Z coordinate (swapped from the source file's Y).
	};

	/**
	 * @struct Map
	 * @brief Represents a full map file, including its name, author, and all coordinate points.
	 */
	struct Map
	{
		std::string name; //!< The name of the map, derived from the filename.
		std::string author; //!< The author of the map, provided via CLI.
		std::vector<Coordinates> coordinates; //!< A list of all coordinate points in the map.
	};

	// --- JSON Serialization ---

	/**
	 * @brief Serializes a Coordinates struct to a JSON object.
	 * @param j The nlohmann::json object to write to.
	 * @param c The Coordinates object to serialize.
	 */
	void to_json(nlohmann::json& j, const Coordinates& c);

	/**
	 * @brief Serializes a Map struct to a JSON object.
	 * @param j The nlohmann::json object to write to.
	 * @param m The Map object to serialize.
	 */
	void to_json(nlohmann::json& j, const Map& m);
} // namespace kx_converter::Models
