#include "models.h"

namespace Models
{
	void vector_to_json(nlohmann::ordered_json& j, std::vector<Coordinates> coords)
	{
		for(const Coordinates coord : coords)
		{
			nlohmann::ordered_json parsedCoords = {
				{"name", coord.name},
				{"x", coord.x},
				{"y", coord.y},
				{"z", coord.z}
			};
			j.push_back(parsedCoords);
		}
	}

	void to_json(nlohmann::ordered_json& j, const Map& map)
	{
		j = nlohmann::ordered_json{ {"name", map.name}, {"Coordinates", map.coords} };
	}

	void from_json(const nlohmann::ordered_json& j, Map& map)
	{
		j.at("name").get_to(map.name);
		j.at("Coordinates").get_to(map.coords);
	}
}
