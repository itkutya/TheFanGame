#include "FileManager.h"

FileManager& FileManager::getInstance()
{
	static FileManager instance;
	return instance;
}

std::vector<std::string> FileManager::load(const std::string& path) noexcept
{
	std::vector<std::string> data;
	std::ifstream file(path);
	if (file.is_open())
	{
		int line = 0;
		while (std::getline(file, data.emplace_back()))
			++line;
	}
	file.close();
	return data;
}

std::vector<std::string> FileManager::load(std::stringstream&& ss, const char& seperator) noexcept
{
	std::vector<std::string> data;
	int line = 0;
	while (std::getline(ss, data.emplace_back(), seperator))
		++line;
	return data;
}

void FileManager::save(const std::string& path) noexcept
{
	std::ofstream file(path);
	if (file.is_open())
		file << this->s_Account->m_username << '\n' << this->s_Account->m_random << '\n' << this->s_Account->m_rememberme;
	file.close();
}
