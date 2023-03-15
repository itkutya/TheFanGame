#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cstdint>

class SettingsManager
{
	struct Setting
	{
		const std::string type;
		void* value;
	};
public:
	SettingsManager(SettingsManager const&) = delete;
	void operator=(SettingsManager const&) = delete;
	virtual ~SettingsManager() noexcept;

	enum ServerData
	{
		SUCCESS = 0, LVL, XP, XPCAP
	};

	template<class T>
	T& get(const std::string& id) noexcept;

	[[nodiscard]] static SettingsManager& getInstance(const std::string& path = "Settings.ini");
	bool save(const std::string& path) noexcept;
private:
	explicit SettingsManager(const std::string& path) noexcept;
	[[nodiscard]] bool load(const std::string& path) noexcept;

	const Setting getTypeAndValue(const std::vector<std::string>& value) noexcept;

	std::unordered_map<std::string, Setting> m_settings;
	bool m_first = true;
};

template<class T>
inline T& SettingsManager::get(const std::string& id) noexcept
{
	return *static_cast<T*>(this->m_settings[id].value);
}