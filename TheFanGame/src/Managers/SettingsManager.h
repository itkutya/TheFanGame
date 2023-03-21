#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <array>
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

			operator int&() noexcept { return this->m_int; };
			operator bool&() noexcept { return this->m_bool; };
			operator std::uint32_t&() noexcept { return this->m_u32; };
			operator std::uint64_t&() noexcept { return this->m_u64; };
			operator std::string&() noexcept { return this->m_string; };

			std::string& operator=(std::string rhs) noexcept { this->m_string = rhs; return *this; };

			int m_int = 0;
			bool m_bool;
			std::uint32_t m_u32;
			std::uint64_t m_u64;
			std::string m_string;
		};

		Setting(const TYPE t) noexcept : type(t) {};
		Setting(const Setting& other) noexcept : type(other.type) 
		{
			switch (other.type)
			{
			case SettingsManager::Setting::INT:
				this->value.m_int = other.value.m_int;
				break;
			case SettingsManager::Setting::BOOL:
				this->value.m_bool = other.value.m_bool;
				break;
			case SettingsManager::Setting::STRING:
				new (&this->value.m_string) std::string(other.value.m_string);
				break;
			case SettingsManager::Setting::U32:
				this->value.m_u32 = other.value.m_u32;
				break;
			case SettingsManager::Setting::U64:
				this->value.m_u64 = other.value.m_u64;
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

	[[nodiscard]] auto& operator[](const std::string& id) noexcept { return this->m_settings[id]; };

	[[nodiscard]] static SettingsManager& getInstance(const std::string& path = "Settings.ini");
	[[nodiscard]] bool save(const std::string& path) noexcept;
	[[nodiscard]] bool load(const std::string& path) noexcept;
private:
	explicit SettingsManager(const std::string& path) noexcept;

	const Setting::TYPE convertStrintToType(const std::string& type) noexcept;
	const std::string convertTypeToString(const Setting::TYPE& type) noexcept;
	const std::string trim(std::string& string) noexcept;

	std::unordered_map<std::string, std::unordered_map<std::string, Setting::Value>> m_settings;
	bool m_first = true;
};