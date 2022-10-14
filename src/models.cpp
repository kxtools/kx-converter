#include "models.h"

namespace Models
{
	void vector_to_json(nlohmann::ordered_json& j, std::vector<Coordinates> coords)
	{
		for(const Coordinates coord : coords)
		{
			nlohmann::ordered_json parsedCoords = {
				{"Name", coord.name},
				{"X", coord.x},
				{"Y", coord.y},
				{"Z", coord.z}
			};
			j.push_back(parsedCoords);
		}
	}

	void to_json(nlohmann::ordered_json& j, const Map& map)
	{
		j = nlohmann::ordered_json{ {"Name", map.name}, {"Coordinates", map.coords} };
	}

	void from_json(const nlohmann::ordered_json& j, Map& map)
	{
		j.at("Name").get_to(map.name);
		j.at("Coordinates").get_to(map.coords);
	}
}
