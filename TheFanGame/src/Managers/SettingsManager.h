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
		union Value
		{
			~Value() noexcept {};

			std::string& operator=(std::string rhs) noexcept { this->m_string = rhs; return this->m_string; };

			std::string m_string = "null";
			int m_int;
			bool m_bool;
			std::uint32_t m_u32;
			std::uint64_t m_u64;
		};

		Setting() noexcept : type("string") {};
		Setting(const std::string& t) noexcept : type(t) {};
		Setting(const Setting& other) noexcept : type(other.type)
		{
			if (this->type == "int")
				this->value.m_int = other.value.m_int;
			else if (this->type == "bool")
				this->value.m_bool = other.value.m_bool;
			else if (this->type == "string")
				new (&this->value.m_string) std::string(other.value.m_string);
			else if (this->type == "u32")
				this->value.m_u32 = other.value.m_u32;
			else if (this->type == "u64")
				this->value.m_u64 = other.value.m_u64;
		}
		~Setting() noexcept
		{
			if (this->type == "string")
				this->value.m_string.~basic_string();
		};

		operator int& () noexcept { return this->value.m_int; };
		operator bool& () noexcept { return this->value.m_bool; };
		operator std::uint32_t& () noexcept { return this->value.m_u32; };
		operator std::uint64_t& () noexcept { return this->value.m_u64; };
		operator std::string& () noexcept { return this->value.m_string; };

		const std::string type;
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

	const std::string trim(std::string& string) noexcept;

	std::unordered_map<std::string, std::unordered_map<std::string, Setting>> m_settings;
	bool m_first = true;
};