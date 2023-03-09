#include "Settings.h"

Settings& Settings::getInstance()
{
	static Settings instance;
	return instance;
}

std::string& Settings::operator[](std::size_t idx) noexcept
{
	return this->m_settings[idx];
}

bool Settings::load(const std::string& path) noexcept
{
	if (this->m_settings = this->s_FileManager->load(path); this->m_settings.size())
		return true;
	return false;
}