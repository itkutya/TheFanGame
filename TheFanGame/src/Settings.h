#pragma once

#include "Managers/FileManager.h"

class Settings
{
public:
	Settings(Settings const&) = delete;
	void operator=(Settings const&) = delete;
	virtual ~Settings() noexcept = default;

	[[nodiscard]] static Settings& getInstance();

	enum FileNumber
	{
		USERNAME = 0, RANDOM, REMEMBERME
	};
	enum ServerData
	{
		SUCCESS = 0, LVL, XP, XPCAP
	};

	std::string& operator[](std::size_t idx) noexcept;

	[[nodiscard]] bool load(const std::string& path) noexcept;
private:
	explicit Settings() noexcept = default;

	FileManager* s_FileManager = &FileManager::getInstance();
	
	std::vector<std::string> m_settings;
};