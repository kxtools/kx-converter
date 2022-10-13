#include <filesystem>
#include <chrono>
#include "utils.h"

int main()
{
	//std::string fileName;
	//int lineNum = 0;
	//
	//spdlog::info("Enter filename: ");
	//getline(std::cin, fileName);
	//if (!Utils::ExistsFile(fileName))
	//{
	//	spdlog::error("File '{}' does not exist.", fileName);
	//	return 1;
	//}
	//spdlog::info("Enter line num(1-n): ");
	//std::cin >> lineNum;
	//// spdlog::info("File total lines: {}", GetLineSize(fileName));
	//
	//std::string outLine;
	//if (!Utils::ReadLine(fileName, lineNum, outLine))
	//{
	//	spdlog::error("Program was unable to execute correctly.");
	//	return 1;
	//}
	//
	//const std::vector<double> pos = Utils::GetPosition(outLine);
	//if (pos.empty())
	//	return 1;
	//const std::vector<double> newPos = Utils::SwitchYZ(pos);
	//if (newPos.empty())
	//	return 1;
	//const std::string firstPart = Utils::GetFirstPart(outLine);
	//const std::string secondPart = Utils::ConvertSecondPart(newPos);
	//const std::string final = firstPart + secondPart;
	//spdlog::info("X: {}, Y: {}, Z: {}", pos[0], pos[1], pos[2]);
	//spdlog::info("X: {}, SY: {}, SZ: {}", newPos[0], newPos[1], newPos[2]);
	//spdlog::info(final);

	std::string pathToDirectory;
	std::string printChangedLinesStr = "n";
	bool printChangedLines;
	spdlog::info("Enter the path of the directory: ");
	std::getline(std::cin, pathToDirectory);
	if (!std::filesystem::is_directory(pathToDirectory))
	{
		spdlog::error("Path '{}' is invalid", pathToDirectory);
		return 1;
	}

	spdlog::info("Print changed lines?(y/n): ");
	std::cin >> printChangedLinesStr;
	printChangedLines = printChangedLinesStr == "n" ? false : true;

	auto t1 = std::chrono::high_resolution_clock::now();
	const std::vector<std::string> filesInDir = Utils::GetFilesInDirectory(pathToDirectory);
	for (const std::string fileName : filesInDir)
	{
		if (fileName.find("map_names.json") != std::string::npos)
			continue;

		// My lines
		std::vector<std::string> lines = Utils::ReadLines(fileName);
		for (const std::string line : lines)
		{
			const std::string firstPart = Utils::GetFirstPart(line);
			const std::vector<double> pos = Utils::GetPosition(line, fileName);
			if (pos.empty())
				return 1;
			const std::vector<double> newPos = Utils::SwitchYZ(pos);
			if (newPos.empty())
				return 1;
			const std::string secondPart = Utils::ConvertSecondPart(newPos);
			const std::string final = firstPart + secondPart;
			if (printChangedLines)
				spdlog::info("{}, {}, {}", fileName, final, line);
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	spdlog::info("Program took {} {} to execute", std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count(),
	             "seconds");
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
