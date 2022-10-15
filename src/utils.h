#pragma once

#include <fstream>
#include <Windows.h>
#include <iostream>
#include "spdlog/spdlog.h"

namespace Utils
{
	static int formatError = 0;


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
		return currentLine - 1; // ignore last space
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

	inline void RemoveString(std::string& str, std::string toRemove)
	{
		std::string::size_type i = str.find(toRemove);
		if (i != std::string::npos)
			str.erase(i, toRemove.length());
	}


	inline std::vector<double> GetPosition(const std::string line, const std::string file)
	{
		// Fixes possible problem with \t
		const std::vector<std::string> lineSplit = split(line, "\t");
		std::vector<double> outVec(3);
		std::vector<double> errVec(4);
		// if it's more than two -> there is more \t if it's less than two -> there is either only 1 \t and then 1 thing before and 0 after or there isn't \t
		if (lineSplit.size() != 2)
		{
			std::cout << std::endl;
			spdlog::critical("While reading file '{}' on the line '{}' a formatting error was found", file, line);
			formatError++;
			return errVec;
		}
		const std::vector<std::string> splitters = split(lineSplit[1], " ");
		if(splitters.size() != 4)
		{
			std::cout << std::endl;
			spdlog::critical("Found unwanted space while reading file '{}' on the line '{}'", file, line);
			formatError++;
			return errVec;
		}
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

	inline std::string GetCheckpointName(std::string line)
	{
		// Split the checkpoint number from the name and position, 2 elements: num, names & xyz
		std::string name;
		const std::vector<std::string> lineSplit = split(line, "\t");
		// Formatting error
		if (lineSplit.size() != 2)
			return name;
		// Split by spaces to have 4 elements: name, x, y, z
		name = split(lineSplit[1], " ")[0];
		// Remove the ' from the 'name'
		name.erase(std::remove(name.begin(), name.end(), '\''), name.end());

		return name;
	}

	inline int GetCheckpoint(std::string line)
	{
		// Split the checkpoint number from the name and position, 2 elements: num, names & xyz
		const std::vector<std::string> lineSplit = split(line, "\t");
		int checkpoint = std::stoi(lineSplit[0]);
		// Formatting error
		if (lineSplit.size() != 2)
			return checkpoint;
		return checkpoint;
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

	inline std::vector<std::string> GetFilesInDirectory(const std::string& directory)
	{
		std::vector<std::string> out;
		for (std::filesystem::recursive_directory_iterator i(directory), end; i != end; ++i)
			if (!is_directory(i->path()))
				out.push_back(i->path().generic_string());
		return out;
	}

	inline bool CreateNewFile(std::string name)
	{
		std::fstream file;
		file.open(name, std::ios::out);
		if (!file)
		{
			spdlog::error("Failed in creating file '{}'", name);
			return false;
		}
		file.close();
		return true;
	}
};
