#include <filesystem>
#include <chrono>
#include "../libs/nlohmann/json.hpp"
#include "models.h"
#include "utils.h"

int main()
{
	std::string srcDirectory;
	spdlog::info("Enter the path of the source directory: ");
	std::getline(std::cin, srcDirectory);

	// Uses filesystem to convert all \\ to /
	srcDirectory = std::filesystem::path(srcDirectory).generic_string();
	if (!std::filesystem::is_directory(srcDirectory))
	{
		spdlog::error("Path '{}' is invalid", srcDirectory);
		return 1;
	}

	// Get directory path and remove the last dir, then append new_maps
	std::string directoryPath = srcDirectory.substr(0, srcDirectory.find_last_of("\\/"));
	directoryPath.append("/new_maps");
	std::filesystem::create_directories(directoryPath);

	auto t1 = std::chrono::high_resolution_clock::now();

	// Loop trough all files in the src dir
	const std::vector<std::string> filesInDir = Utils::GetFilesInDirectory(srcDirectory);
	for (const std::string fileName : filesInDir)
	{
		if (fileName.find("map_names.json") != std::string::npos)
			continue;

		// Get mid path - the path between srcDir and fileName
		std::string midPath = fileName;
		Utils::RemoveString(midPath, std::filesystem::path(fileName).filename().generic_string());
		Utils::RemoveString(midPath, srcDirectory);

		// Create the dir for midPath if it doesn't exist
		if (!std::filesystem::is_directory(directoryPath + midPath) || 
			!std::filesystem::exists(directoryPath + midPath))
			std::filesystem::create_directories(directoryPath + midPath);

		// Create a file with with the filename path + json
		std::string file = directoryPath + midPath + std::filesystem::path(fileName).stem().generic_string() + ".json";
		if (!Utils::CreateNewFile(file))
			break;


		nlohmann::ordered_json resultCoords;
		std::vector<Models::Coordinates> coords;
		nlohmann::ordered_json finalJson;

		// Read lines of the files and loop trough them
		std::vector<std::string> lines = Utils::ReadLines(fileName);
		for (const std::string line : lines)
		{
			// Get position and convert the Y & Z coordinate
			const std::vector<double> pos = Utils::GetPosition(line, fileName);
			if (pos.empty())
				return 1;
			const std::vector<double> newPos = Utils::SwitchYZ(pos);
			if (newPos.empty())
				return 1;
			const std::string name = Utils::GetCheckpointName(line);
			coords.push_back(Models::Coordinates{name, newPos[0], newPos[1], newPos[2]});
		}
		// Convert the coords into a json::array()
		vector_to_json(resultCoords, coords);
		Models::Map newMap;
		newMap.coords = resultCoords;
		newMap.name = std::filesystem::path(fileName).stem().generic_string();
		to_json(finalJson, newMap);

		// Dump into a file while ignoring utf-8
		// https://json.nlohmann.me/home/exceptions/#jsonexceptiontype_error316
		// https://json.nlohmann.me/api/basic_json/error_handler_t/#examples
		std::ofstream f(file, std::ios_base::trunc | std::ios_base::out);
		f << finalJson.dump(4, ' ', false, nlohmann::json::error_handler_t::ignore) << std::endl;
	}
	spdlog::info("Program took {} {} to execute",
	             std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - t1).
	             count(),
	             "seconds");
	return 0;
}

/* TODO: 0. Think about precision, it has more decimal places than needed ✅
 * TODO: 1. Convert to CMake ✅
 * TODO: 2. Use classes/namespaces ✅
 * TODO: 3. Make it choose a whole folder ✅
 * TODO: 4. Create a new folder with the changed files ✅
 * TODO: 5. Change the files to json ✅
*/
