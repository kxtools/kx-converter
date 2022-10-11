#include <iostream>
#include <fstream>
#include "spdlog/spdlog.h"

bool ExistsFile(std::string fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (file.fail())
	{
		file.close();
		return false;
	}
	file.close();
	return true;
}

int GetLineSize(std::string fileName)
{
	// Dumb
	std::ifstream file;
	file.open(fileName);
	int currentLine = 0;
	std::string line;

	while (!file.eof())
	{
		currentLine++;
		getline(file, line);
	}
	file.close();
	return currentLine;
}

bool ReadLine(std::string fileName, int lineNum, std::string& outLine)
{
	std::ifstream file;
	lineNum = lineNum <= 0 ? 1 : lineNum;
	file.open(fileName);
	if (file.fail())
	{
		file.close();
		spdlog::error("Failed to open '{}'", fileName);
		return false;
	}

	int currentLine = 0;
	std::string line;

	while (!file.eof())
	{
		currentLine++;
		getline(file, line);
		if (currentLine == lineNum) break;
	}

	if (currentLine < lineNum)
	{
		file.close();
		spdlog::error("Line number reached the maximum line size of the file. Total Lines: {}", currentLine);
		return false;
	}
	spdlog::info(line);
	file.close();
	outLine = line;
	return true;
}

std::vector<std::string> ReadLines(std::string fileName)
{
	std::ifstream file;
	const int maxLines = GetLineSize(fileName);
	int currentLine = 0;
	std::vector<std::string> lines(maxLines);

	file.open(fileName);
	if (file.fail())
	{
		file.close();
		spdlog::error("Failed to open '{}'", fileName);
		return lines;
	}

	while (!file.eof())
	{
		currentLine++;
		getline(file, lines[currentLine]);
	}
	file.close();
	return lines;
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	}
	while (pos < str.length() && prev < str.length());
	return tokens;
}

std::vector<double> GetPosition(std::string line)
{
	const std::vector<std::string> splitters = split(line, " ");
	std::vector<double> outVec(3);
	if (!splitters.empty())
	{
		for (size_t i = 1; i < splitters.size(); i++)
		{
			outVec[i-1] = std::stod(splitters[i]);
		}
	}
	return outVec;
}

std::vector<double> SwitchYZ(std::vector<double> vec)
{
	std::vector<double> outVec(3);
	if (vec.empty())
		return outVec;
	outVec[0] = vec[0];
	outVec[1] = vec[2];
	outVec[2] = vec[1];
	return outVec;
}

std::string GetFirstPart(std::string line)
{
	const std::vector<std::string> splitters = split(line, " ");
	return splitters[0] + " ";
}

std::string ConvertSecondPart(std::vector<double> newPos)
{
	std::string outStr;
	if (newPos.empty())
		return "ERROR: SECOND PART";
	for(size_t i = 0; i < newPos.size(); i++)
	{
		outStr.append(std::to_string(newPos[i]));
		outStr.append(" ");
	}
	return outStr;
}

int main()
{
	std::string fileName;
	int lineNum = 0;

	spdlog::info("Enter filename: ");
	std::cin >> fileName;
	if (!ExistsFile(fileName))
	{
		spdlog::error("File '{}' does not exist.", fileName);
		return 1;
	}
	spdlog::info("Enter line num(1-n): ");
	std::cin >> lineNum;
	// spdlog::info("File total lines: {}", GetLineSize(fileName));

	std::string outLine;
	if (!ReadLine(fileName, lineNum, outLine))
	{
		spdlog::error("Program was unable to execute correctly.");
		return 1;
	}

	const std::vector<double> pos = GetPosition(outLine);
	if (pos.empty())
		return 1;
	const std::vector<double> newPos = SwitchYZ(pos);
	if (newPos.empty())
		return 1;
	const std::string firstPart = GetFirstPart(outLine);
	const std::string secondPart = ConvertSecondPart(newPos);
	const std::string final = firstPart + secondPart;
	spdlog::info("X: {}, Y: {}, Z: {}", pos[0], pos[1], pos[2]);
	spdlog::info("X: {}, SY: {}, SZ: {}", newPos[0], newPos[1], newPos[2]);
	spdlog::info(final);
	return 0;
}

/* TODO: 1. Convert to CMake
 * TODO: 2. Use classes
 * TODO: 3. Make it choose a whole folder and change the files from there
*/