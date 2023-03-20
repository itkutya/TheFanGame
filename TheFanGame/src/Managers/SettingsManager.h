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
		enum TYPE
		{
			INT, BOOL, STRING, U32, U64
		};
		union Value
		{
			~Value() noexcept {};

			int m_int = 0;
			bool m_bool;
			std::uint32_t m_u32;
			std::uint64_t m_u64;
			std::string m_string;
		};

		Setting() noexcept : type(TYPE::BOOL) {};
		Setting(const TYPE t) noexcept : type(t) {};
		Setting(const Setting& other) noexcept : type(other.type) 
		{
			switch (other.type)
			{
			case SettingsManager::Setting::INT:
				value.m_int = other.value.m_int;
				break;
			case SettingsManager::Setting::BOOL:
				value.m_bool = other.value.m_bool;
				break;
			case SettingsManager::Setting::STRING:
				new (&value.m_string) std::string(other.value.m_string);
				break;
			case SettingsManager::Setting::U32:
				value.m_u32 = other.value.m_u32;
				break;
			case SettingsManager::Setting::U64:
				value.m_u64 = other.value.m_u64;
				break;
			}
		};
		~Setting() noexcept 
		{
			if (this->type == TYPE::STRING)
				this->value.m_string.~basic_string();
		};

		const TYPE type;
		Value value;
	};
public:
	SettingsManager(SettingsManager const&) = delete;
	void operator=(SettingsManager const&) = delete;
	virtual ~SettingsManager() noexcept = default;

	enum ServerData
	{
		SUCCESS = 0, LVL, XP, XPCAP
	};

	[[nodiscard]] Setting::Value& get(const std::string& id) noexcept
	{
		return this->m_settings[id].value;
	};

	[[nodiscard]] static SettingsManager& getInstance(const std::string& path = "Settings.ini");
	[[nodiscard]] bool save(const std::string& path) noexcept;
private:
	explicit SettingsManager(const std::string& path) noexcept;
	[[nodiscard]] bool load(const std::string& path) noexcept;

	const Setting getTypeAndValue(const std::vector<std::string>& value) noexcept;

	std::unordered_map<std::string, Setting> m_settings;
	bool m_first = true;
};