#include "FileManager.h"

#include <fstream>
#include <array>

const std::vector<FileManager::ParsedBranch> FileManager::parseFile(const char* filepath) noexcept
{
	std::vector<FileManager::ParsedBranch> parsedString;
	std::ifstream is(filepath);
	if (is.is_open())
	{
		while (!is.eof())
		{
			std::string data;
			is >> data;
			if (data.length())
			{
				if (data[0] == '[' || data[data.length() - 1] == ']')
					parsedString.emplace_back(std::make_pair<const std::string, std::vector<FileManager::ParsedSetting>>(this->trim(data), {}));
				else
				{
					std::array<std::string, 2> setting;
					is >> setting[0] >> setting[1];
					if (parsedString.size())
						parsedString.back().second.emplace_back(data, setting[0], setting[1]);
				}
			}
		}
	}
	return parsedString;
}

const bool FileManager::saveToFile(const char* filepath, std::unordered_map<std::string, std::unordered_map<std::string, Setting>>& data) noexcept
{
	std::ofstream file(filepath, std::ios_base::trunc);
	bool succes = false;
	if (succes = file.is_open())
	{
		for (const auto& mainbranch : data)
		{
			file << '[' << mainbranch.first << ']' << '\n';
			for (auto& [secondbranch, setting] : data.at(mainbranch.first))
				file << setting.m_type << ' ' << secondbranch << ' ' << setting.toString() << '\n';
		}
	}
	file.close();
	return succes;
}

const std::string FileManager::trim(const std::string s) noexcept
{
	return { s.begin() + 1, s.end() - 1 };
}