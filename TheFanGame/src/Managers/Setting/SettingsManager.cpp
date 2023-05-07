#include "SettingsManager.h"

SettingsManager& SettingsManager::getInstance(const char* path)
{
	static SettingsManager instance(path);
	return instance;
}

const bool SettingsManager::save(const char* path) noexcept
{
	FileManager fm;
	return fm.saveToFile(path, this->m_settings);
}

SettingsManager::SettingsManager(const char* path)
{
	if (!this->load(path))
		throw "Could not load settings!";
}

const bool SettingsManager::load(const char* path) noexcept
{
	FileManager fm;
	const auto& parse = fm.parseFile(path);
	if (parse.empty())
		return false;
	for (const auto& mainchannel : parse)
	{
		auto& mainbranch = this->m_settings.emplace(mainchannel.first, secondBranch()).first->second;
		for (const auto& [type, name, value] : mainchannel.second)
		{
			auto& setting = mainbranch.emplace(name, Setting(type)).first->second;
			setting.setValue(value);
		}
	}
	return true;
}