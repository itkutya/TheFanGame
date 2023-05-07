#pragma once

#include "Managers/FileManager.h"

class SettingsManager : NonCopyable
{
	typedef std::unordered_map<std::string, Setting> secondBranch;
	typedef std::string mainBranch;
public:
	~SettingsManager() noexcept = default;

	[[nodiscard]] auto& operator[](const std::string id) noexcept { return this->m_settings.at(id); };

	[[nodiscard]] static SettingsManager& getInstance(const char* path);
	[[nodiscard]] const bool save(const char* path) noexcept;
private:
	explicit SettingsManager(const char* path);

	[[nodiscard]] const bool load(const char* path) noexcept;

	std::unordered_map<mainBranch, secondBranch> m_settings;
};