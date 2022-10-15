#include <filesystem>
#include <Windows.h>
#include <ShlObj.h>
#include <atlbase.h>
#include <chrono>
#include "../libs/nlohmann/json.hpp"
#include "progress_bar.h"
#include "models.h"
#include "utils.h"

int main()
{
	spdlog::info("Enter the path of the maps directory: ");
	std::string srcDirectory = Utils::BrowseDirectory();

	// Uses filesystem to convert all \\ to /
	srcDirectory = std::filesystem::path(srcDirectory).generic_u8string();
	if (!std::filesystem::is_directory(srcDirectory))
	{
		spdlog::error("Path '{}' is invalid", srcDirectory);
		std::cout << "Press any key to close this window...";
		std::getchar();
		return 1;
	}
	std::cout << srcDirectory << std::endl;

	// Get directory path and then add "-converted" at the end
	std::string directoryPath = srcDirectory.substr(0, srcDirectory.find_last_of("\\/"));
	std::string srcDirName = srcDirectory;
	Utils::RemoveString(srcDirName, directoryPath);
	directoryPath += srcDirName + "-converted";
	std::filesystem::create_directories(directoryPath);

	auto t1 = std::chrono::high_resolution_clock::now();

	// Loop trough all files in the src dir
	const std::vector<std::string> filesInDir = Utils::GetFilesInDirectory(srcDirectory);
	if (std::filesystem::is_empty(srcDirectory))
	{
		spdlog::error("Directory '{}' is empty", srcDirectory);
		std::cout << "Press any key to close this window...";
		std::getchar();
		return 1;
	}

	progressbar bar(static_cast<int>(filesInDir.size()) - 1);
	bool canUpdateBar = true;
	for (const std::string fileName : filesInDir)
	{
		if (std::filesystem::path(fileName).extension() != ".txt")
			continue;

		// Get mid path - the path between srcDir and fileName
		std::string midPath = fileName;
		Utils::RemoveString(midPath, std::filesystem::path(fileName).filename().generic_u8string());
		Utils::RemoveString(midPath, srcDirectory);

		// Create the dir for midPath if it doesn't exist
		if (!std::filesystem::is_directory(directoryPath + midPath) ||
			!std::filesystem::exists(directoryPath + midPath))
			std::filesystem::create_directories(directoryPath + midPath);

		// Create a file with with the filename path + json
		std::string file = directoryPath + midPath + std::filesystem::path(fileName).stem().generic_string() + ".json";
		if (!Utils::CreateNewFile(file))
			break;

		if (canUpdateBar)
			bar.update();

		nlohmann::ordered_json resultCoords;
		std::vector<Models::Coordinates> coords;
		nlohmann::ordered_json finalJson;

		// Read lines of the files and loop trough them
		std::vector<std::string> lines = Utils::ReadLines(fileName);
		for (const std::string line : lines)
		{
			// Get position and convert the Y & Z coordinate
			const std::vector<double> pos = Utils::GetPosition(line, fileName);
			std::vector<double> errVec(4);
			if (pos == errVec)
				canUpdateBar = false;
			const std::vector<double> newPos = Utils::SwitchYZ(pos);
			const std::string name = Utils::GetCheckpointName(line);
			coords.push_back(Models::Coordinates{name, newPos[0], newPos[1], newPos[2]});
		}
		// Convert the coords into a json::array()
		vector_to_json(resultCoords, coords);
		Models::Map newMap;
		newMap.coords = resultCoords;
		newMap.name = std::filesystem::path(fileName).stem().generic_u8string();
		to_json(finalJson, newMap);

		// Dump into a file while ignoring utf-8
		// https://json.nlohmann.me/home/exceptions/#jsonexceptiontype_error316
		// https://json.nlohmann.me/api/basic_json/error_handler_t/#examples
		std::ofstream f(file, std::ios_base::trunc | std::ios_base::out);
		f << finalJson.dump(2, ' ', false, nlohmann::json::error_handler_t::ignore) << std::endl;
	}
	std::cout << std::endl;
	if (Utils::formatError > 0)
		spdlog::error("Found {} formatting errors", Utils::formatError);
	spdlog::info("Program took {} {} to execute",
	             std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - t1).
	             count(),
	             "seconds");
	std::cout << "Press any key to close this window...";
	std::getchar();
	return 0;
}

/* TODO: 0. Think about precision, it has more decimal places than needed ✅
 * TODO: 1. Convert to CMake ✅
 * TODO: 2. Use classes/namespaces ✅
 * TODO: 3. Make it choose a whole folder ✅
 * TODO: 4. Create a new folder with the changed files ✅
 * TODO: 5. Change the files to json ✅
*/
