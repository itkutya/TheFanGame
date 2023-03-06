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

bool FileManager::save(const std::string& path, const std::vector<std::string>& elements) noexcept
{
	bool succes = false;
	std::ofstream file(path);
	if (succes = file.is_open(); succes)
		for (auto& element : elements)
			file << element << '\n';
	file.close();
	return succes;
}