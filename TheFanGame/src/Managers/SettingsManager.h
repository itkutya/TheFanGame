#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <string>

class SettingsManager
{
public:
	SettingsManager(SettingsManager const&) = delete;
	void operator=(SettingsManager const&) = delete;
	virtual ~SettingsManager() noexcept = default;

	enum FileNumber
	{
		USERNAME = 0, RANDOM, REMEMBERME, MUSIC_TITLE
	};
	enum ServerData
	{
		SUCCESS = 0, LVL, XP, XPCAP
	};

	std::string& operator[](std::size_t idx) noexcept;

	[[nodiscard]] static SettingsManager& getInstance(const std::string& path = "Settings.ini");
	bool save(const std::string& path) noexcept;
private:
	explicit SettingsManager(const std::string& path) noexcept;
	[[nodiscard]] bool load(const std::string& path) noexcept;

	std::vector<std::string> m_settings;
	bool m_first = true;
};