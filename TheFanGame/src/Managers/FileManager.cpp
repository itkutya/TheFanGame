#include "FileManager.h"

const std::vector<std::pair<const std::string, std::vector<FileManager::ParsedSetting>>> FileManager::parseFile(const char* filepath) noexcept
{
	std::vector<std::pair<const std::string, std::vector<FileManager::ParsedSetting>>> parsedString;
	this->m_inputstream.open(filepath);
	if (this->m_inputstream.is_open())
	{
		while (!this->m_inputstream.eof())
		{
			std::string data;
			this->m_inputstream >> data;
			if (data.length())
			{
				if (data[0] == '[' || data[data.length() - 1] == ']')
					parsedString.emplace_back(std::make_pair<const std::string, std::vector<FileManager::ParsedSetting>>(this->trim(data), {}));
				else
				{
					std::array<std::string, 2> setting;
					this->m_inputstream >> setting[0] >> setting[1];
					if (parsedString.size())
						parsedString.back().second.emplace_back(data, setting[0].c_str(), setting[1].c_str());
				}
			}
		}
	}
	return parsedString;
}

const std::vector<std::pair<const std::string, std::vector<FileManager::ParsedSetting>>> FileManager::parseFile() noexcept
{
	std::vector<std::pair<const std::string, std::vector<FileManager::ParsedSetting>>> parsedString;
	if (this->m_inputstream.is_open())
	{
		while (!this->m_inputstream.eof())
		{
			std::string data;
			this->m_inputstream >> data;
			if (data.length())
			{
				if (data[0] == '[' || data[data.length() - 1] == ']')
					parsedString.emplace_back(std::make_pair<const std::string, std::vector<FileManager::ParsedSetting>>(this->trim(data.c_str()), {}));
				else
				{
					std::array<std::string, 2> setting;
					this->m_inputstream >> setting[0] >> setting[1];
					if (parsedString.size())
						parsedString.back().second.emplace_back(data.c_str(), setting[0].c_str(), setting[1].c_str());
				}
			}
		}
	}
	return parsedString;
}

const bool FileManager::saveToFile(const char* filepath) noexcept
{
	return false;
}

const std::string FileManager::trim(const std::string sw) noexcept
{
	return { sw.begin() + 1, sw.end() - 1 };
}