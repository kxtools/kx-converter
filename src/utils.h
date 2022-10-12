#pragma once

#include <fstream>
#include <Windows.h>
#include <iostream>
#include "spdlog/spdlog.h"

namespace Utils
{
	static int formatError = 0;

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
		return currentLine - 1;
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
			if (currentLine >= maxLines) break;
			getline(file, lines[currentLine]);
			currentLine++;
		}
		file.close();
		return lines;
	}

	inline std::vector<std::string> split(std::string phrase, std::string delimiter)
	{
		std::vector<std::string> list;
		auto s = std::string(phrase);
		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			token = s.substr(0, pos);
			list.push_back(token);
			s.erase(0, pos + delimiter.length());
		}
		list.push_back(s);
		return list;
	}

	inline std::vector<double> GetPosition(const std::string line, const std::string file)
	{
		// Fixes possible problem with \t
		const std::vector<std::string> lineSplit = split(line, "\t");
		std::vector<double> outVec(3);
		// if it's more than two -> there is more \t if it's less than two -> there is either only 1 \t and then 1 thing before and 0 after or there isn't \t
		if (lineSplit.size() != 2)
		{
			spdlog::critical("{}, {}", line, file);
			formatError++;
			spdlog::error("{}", formatError);
			return outVec;
		}
		const std::vector<std::string> splitters = split(lineSplit[1], " ");
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
		for (const auto i : newPos)
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

	inline std::vector<std::string> GetFilesInDirectory(const std::string& directory)
	{
		std::vector<std::string> out;
		for (std::filesystem::recursive_directory_iterator i(directory), end; i != end; ++i)
			if (!is_directory(i->path()))
				out.push_back(i->path().generic_string());
		return out;
	}
};
