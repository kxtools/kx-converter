#include <filesystem>
#include <chrono>
#include "../libs/nlohmann/json.hpp"
#include "models.h"
#include "utils.h"

int main()
{
	/*
	 *
{
    "name": "example name",
    "description": "example description",
    "mapId": 15,
    "author": "me",
    "coordinates": [
        {
            "name": "example name1",
            "x": 100.00,
            "y": 100.00,
            "z": 100.00
        },
        {
            "name": "example name2",
            "x": 200.00,
            "y": 200.00,
            "z": 200.00
        },
        {
            "name": "example name3",
            "x": 300.00,
            "y": 300.00,
            "z": 300.00
        }
    ]
}
	json j = {
		{"name", "example name"},
		{"coordinates", {{"name", "example name"}, {"x", 100.00}, {"y", 100.00}, {"z", 100.00}}, {{"name", "example name2"}, {"x", 200.00}, {"y", 200.00}, {"z", 200.00}}, {{"name", "example name3"}, {"x", 300.00}, {"y", 300.00}, {"z", 300.00}}}
	}
	 */
	// std::string pathToDirectory;
	// std::string printChangedLinesStr = "n";
	// bool printChangedLines;
	// spdlog::info("Enter the path of the directory: ");
	// std::getline(std::cin, pathToDirectory);
	// if (!std::filesystem::is_directory(pathToDirectory))
	// {
	// 	spdlog::error("Path '{}' is invalid", pathToDirectory);
	// 	return 1;
	// }
	// 
	// spdlog::info("Print changed lines?(y/n): ");
	// std::cin >> printChangedLinesStr;
	// printChangedLines = printChangedLinesStr == "n" ? false : true;
	// 
	// auto t1 = std::chrono::high_resolution_clock::now();
	// const std::vector<std::string> filesInDir = Utils::GetFilesInDirectory(pathToDirectory);
	// for (const std::string fileName : filesInDir)
	// {
	// 	if (fileName.find("map_names.json") != std::string::npos)
	// 		continue;
	// 
	// 	// My lines
	// 	std::vector<std::string> lines = Utils::ReadLines(fileName);
	// 	for (const std::string line : lines)
	// 	{
	// 		const std::string firstPart = Utils::GetFirstPart(line);
	// 		const std::vector<double> pos = Utils::GetPosition(line, fileName);
	// 		if (pos.empty())
	// 			return 1;
	// 		const std::vector<double> newPos = Utils::SwitchYZ(pos);
	// 		if (newPos.empty())
	// 			return 1;
	// 		const std::string secondPart = Utils::ConvertSecondPart(newPos);
	// 		const std::string final = firstPart + secondPart;
	// 		if (printChangedLines)
	// 			spdlog::info("{}, {}, {}", fileName, final, line);
	// 	}
	// }
	// auto t2 = std::chrono::high_resolution_clock::now();
	// spdlog::info("Program took {} {} to execute", std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count(),
	//              "seconds");
	Utils::CreateNewFile("test.json");
	// nlohmann::json j = {
	// {"name", "example name"},
	// {"coordinates", {{"name", "example name"}, {"x", 100.00}, {"y", 100.00}, {"z", 100.00}}, {{"name", "example name2"}, {"x", 200.00}, {"y", 200.00}, {"z", 200.00}}, {{"name", "example name3"}, {"x", 300.00}, {"y", 300.00}, {"z", 300.00}}}
	// };


	// nlohmann::json j = nlohmann::json
	// {
	// 	{"name", "Example name"},
	// 	{"Coordinates", result},
	// 	{"test", "1"}
	// };
	nlohmann::ordered_json result = nlohmann::json::array();
	std::vector<Models::Coordinates> coords;
	nlohmann::ordered_json finalJson;

	coords.push_back(Models::Coordinates{ "example name", 1, 1, 1 });
	Models::vector_to_json(result, coords);
	Models::Map newMap;
	newMap.coords = result;
	newMap.name = "Example name";

	Models::to_json(finalJson, newMap);

	std::ofstream f("test.json", std::ios_base::trunc | std::ios_base::out);
	f << std::setw(4) << finalJson << std::endl;
	return 0;
}

// TODO: Think about precision, it is:   ✅
// 2      'PoI2' 267.509 164.109 608.056
// to
// 2      'PoI2' 267.509000 608.056000 164.109000

/* TODO: 1. Convert to CMake ✅
 * TODO: 2. Use classes/namespaces ✅
 * TODO: 3. Make it choose a whole folder ✅
 * TODO: 4. Create a new folder with the changed files
 * TODO: 5. Change the files to json
*/
