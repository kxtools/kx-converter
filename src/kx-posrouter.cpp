#include "utils.h"

int main()
{
	std::string fileName;
	int lineNum = 0;

	spdlog::info("Enter filename: ");
	std::cin >> fileName;
	if (!Utils::ExistsFile(fileName))
	{
		spdlog::error("File '{}' does not exist.", fileName);
		return 1;
	}
	spdlog::info("Enter line num(1-n): ");
	std::cin >> lineNum;
	// spdlog::info("File total lines: {}", GetLineSize(fileName));

	std::string outLine;
	if (!Utils::ReadLine(fileName, lineNum, outLine))
	{
		spdlog::error("Program was unable to execute correctly.");
		return 1;
	}

	const std::vector<double> pos = Utils::GetPosition(outLine);
	if (pos.empty())
		return 1;
	const std::vector<double> newPos = Utils::SwitchYZ(pos);
	if (newPos.empty())
		return 1;
	const std::string firstPart = Utils::GetFirstPart(outLine);
	const std::string secondPart = Utils::ConvertSecondPart(newPos);
	const std::string final = firstPart + secondPart;
	spdlog::info("X: {}, Y: {}, Z: {}", pos[0], pos[1], pos[2]);
	spdlog::info("X: {}, SY: {}, SZ: {}", newPos[0], newPos[1], newPos[2]);
	spdlog::info(final);
	return 0;
}

// TODO: Think about precision, it is:   ✅
// 2      'PoI2' 267.509 164.109 608.056
// to
// 2      'PoI2' 267.509000 608.056000 164.109000

/* TODO: 1. Convert to CMake ✅
 * TODO: 2. Use classes/namespaces ✅
 * TODO: 3. Make it choose a whole folder and change the files from there
*/
