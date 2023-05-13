#pragma once

#include "Utility.h"
#include "Managers/FileManager.h"

class SettingsManager : public Singleton<SettingsManager>
{
	friend class Singleton<SettingsManager>;

	typedef std::unordered_map<std::string, Setting> secondBranch;
	typedef std::string mainBranch;
protected:
	SettingsManager();
	~SettingsManager() noexcept = default;
public:
	[[nodiscard]] auto& operator[](const std::string id) noexcept { return this->m_settings.at(id); };

	[[nodiscard]] const bool save(const char* path) noexcept;
	[[nodiscard]] const bool load(const char* path) noexcept;
private:
	std::unordered_map<mainBranch, secondBranch> m_settings;
};