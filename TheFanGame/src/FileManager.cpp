#include "FileManager.h"

FileManager& FileManager::getInstance()
{
	static FileManager instance;
	return instance;
}

std::vector<std::string> FileManager::load(const std::string& path, const char& seperator) noexcept
{
	std::vector<std::string> data;
	std::ifstream file(path);
	int line = 0;
	if (file.is_open())
	{
		while (std::getline(file, data.emplace_back(), seperator))
			++line;
	}
	else
	{
		std::stringstream ss(path);
		while (std::getline(ss, data.emplace_back(), seperator))
			++line;
	}
	file.close();
	return data;
}

void FileManager::save(const std::string& path) noexcept
{
	std::ofstream file(path);
	if (file.is_open())
		file << this->s_Account->m_username << '\n' << this->s_Account->m_random << '\n' << this->s_Account->m_rememberme;
	file.close();
}
