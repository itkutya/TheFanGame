#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>
#include <any>
#include <cstdint>
#include <type_traits>

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

	std::any& operator[](const std::string& id) noexcept;

	[[nodiscard]] static SettingsManager& getInstance(const std::string& path = "Settings.ini");
	bool save(const std::string& path) noexcept;
private:
	explicit SettingsManager(const std::string& path) noexcept;
	[[nodiscard]] bool load(const std::string& path) noexcept;

	const std::any getTypeAndValue(const std::vector<std::string>& value) noexcept;
	const std::string getUnderLyingType(std::any& type) noexcept;

	std::unordered_map<std::string, std::any> m_settings;
	bool m_first = true;
};