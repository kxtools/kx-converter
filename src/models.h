#pragma once

#include <string>
#include "../libs/nlohmann/json.hpp"

namespace Models
{
	struct Coordinates
	{
		std::string name;
		double x{0.0};
		double y{0.0};
		double z{0.0};
	};

	struct Map
	{
		std::string name;
		nlohmann::ordered_json coords{nlohmann::ordered_json::array()};
	};

	void vector_to_json(nlohmann::ordered_json& j, std::vector<Coordinates> coords);
	void to_json(nlohmann::ordered_json& j, const Map& map);
	void from_json(const nlohmann::ordered_json& j, Map& map);
}
