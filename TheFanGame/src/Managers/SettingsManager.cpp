#include "SettingsManager.h"

std::string& SettingsManager::operator[](std::size_t idx) noexcept
{
	return this->m_settings[idx];
}

SettingsManager& SettingsManager::getInstance(const std::string& path)
{
	static SettingsManager instance(path);
	return instance;
}

bool SettingsManager::save(const std::string& path) noexcept
{
	bool succes = false;
	std::ofstream file(path);
	if (succes = file.is_open(); succes)
		for (std::size_t i = 0; i < this->m_settings.size(); ++i)
		{
			if (i != this->m_settings.size() - 1)
				file << this->m_settings[i] << '\n';
			else
				file << this->m_settings[i];
		}
	file.close();
	return succes;
}

SettingsManager::SettingsManager(const std::string& path) noexcept
{
	if (this->m_first && this->load(path))
		this->m_first = false;
}

bool SettingsManager::load(const std::string& path) noexcept
{
	bool success = false;
	std::ifstream file(path);
	if (success = file.is_open(); success)
		while (std::getline(file, this->m_settings.emplace_back()));
	while (this->m_settings[this->m_settings.size() - 1] == "")
		this->m_settings.pop_back();
	file.close();
	return success;
}