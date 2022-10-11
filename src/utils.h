#pragma once

#include <iostream>
#include <fstream>
#include "spdlog/spdlog.h"

namespace Utils
{
	inline bool ExistsFile(const std::string fileName)
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

	inline int GetLineSize(const std::string fileName)
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

	inline bool ReadLine(const std::string fileName, int lineNum, std::string& outLine)
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

	inline std::vector<std::string> ReadLines(const std::string fileName)
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

	inline std::vector<std::string> split(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			std::string token = str.substr(prev, pos - prev);
			if (!token.empty()) tokens.push_back(token);
			prev = pos + delim.length();
		}
		while (pos < str.length() && prev < str.length());
		return tokens;
	}

	inline std::vector<double> GetPosition(const std::string line)
	{
		const std::vector<std::string> splitters = split(line, " ");
		std::vector<double> outVec(3);
		if (!splitters.empty())
		{
			for (size_t i = 1; i < splitters.size(); i++)
			{
				outVec[i - 1] = std::stod(splitters[i]);
			}
		}
		return outVec;
	}

	inline std::vector<double> SwitchYZ(std::vector<double> vec)
	{
		std::vector<double> outVec(3);
		if (vec.empty())
			return outVec;
		outVec[0] = vec[0];
		outVec[1] = vec[2];
		outVec[2] = vec[1];
		return outVec;
	}

	inline std::string GetFirstPart(const std::string line)
	{
		const std::vector<std::string> splitters = split(line, " ");
		return splitters[0] + " ";
	}

	inline std::string ConvertSecondPart(const std::vector<double> newPos)
	{
		std::string outStr;
		if (newPos.empty())
			return outStr;
		for(const auto i : newPos)
		{
			// Remove trailing 0s
			std::string neuPos = std::to_string(i);
			neuPos.erase(neuPos.find_last_not_of('0') + 1, std::string::npos);
			neuPos.erase(neuPos.find_last_not_of('.') + 1, std::string::npos);
			outStr.append(neuPos);
			outStr.append(" ");
		}
		return outStr;
	}
};
