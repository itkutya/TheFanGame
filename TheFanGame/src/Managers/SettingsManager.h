#pragma once

#include <unordered_map>
#include <memory>

#include "Input/InputTypes.h"
#include "FileManager.h"

class SettingsManager : NonCopyable
{
	struct Setting
	{
		union Value
		{
			~Value() noexcept {};

			std::string& operator=(std::string rhs) noexcept { this->m_string = rhs; return this->m_string; };

			std::string m_string = "Null";
			std::unique_ptr<Input> m_input;
			int m_int;
			bool m_bool;
			std::uint32_t m_u32;
			std::uint64_t m_u64;
		};
		
		Setting() noexcept : type("Null") {};
		Setting(const std::string_view t) noexcept : type(t)
		{
			if (this->type == "int")
				this->value.m_int = 0;
			else if (this->type == "bool")
				this->value.m_bool = false;
			else if (this->type == "string")
				new (&this->value.m_string) std::string("Null");
			else if (this->type == "u32")
				this->value.m_u32 = 0;
			else if (this->type == "u64")
				this->value.m_u64 = 0;
			else if (this->type == "Keyboard")
				new (&this->value.m_input) std::unique_ptr<Keyboard>(std::make_unique<Keyboard>(sf::Keyboard::Scancode::A));
			else if (this->type == "MouseButton")
				new (&this->value.m_input) std::unique_ptr<MouseButton>(std::make_unique<MouseButton>(sf::Mouse::Button::Left));
			else if (this->type == "MouseWheel")
				new (&this->value.m_input) std::unique_ptr<MouseWheel>(std::make_unique<MouseWheel>(sf::Mouse::Wheel::VerticalWheel));
			else if (this->type == "JoystickButton")
				new (&this->value.m_input) std::unique_ptr<JoystickButton>(std::make_unique<JoystickButton>(0));
			else if (this->type == "JoystickAxis")
				new (&this->value.m_input) std::unique_ptr<JoystickAxis>(std::make_unique<JoystickAxis>(sf::Joystick::Axis::X));
		};
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
			else if (this->type == "Keyboard")
				new (&this->value.m_input) std::unique_ptr<Keyboard>(std::make_unique<Keyboard>(static_cast<Keyboard*>(other.value.m_input.get())->m_KeyCode));
			else if (this->type == "MouseButton")
				new (&this->value.m_input) std::unique_ptr<MouseButton>(std::make_unique<MouseButton>(static_cast<MouseButton*>(other.value.m_input.get())->m_MouseButton));
			else if (this->type == "MouseWheel")
				new (&this->value.m_input) std::unique_ptr<MouseWheel>(std::make_unique<MouseWheel>(static_cast<MouseWheel*>(other.value.m_input.get())->m_MouseWheel));
			else if (this->type == "JoystickButton")
				new (&this->value.m_input) std::unique_ptr<JoystickButton>(std::make_unique<JoystickButton>(static_cast<JoystickButton*>(other.value.m_input.get())->m_joystickButton));
			else if (this->type == "JoystickAxis")
				new (&this->value.m_input) std::unique_ptr<JoystickAxis>(std::make_unique<JoystickAxis>(static_cast<JoystickAxis*>(other.value.m_input.get())->m_JoystickAxis));
		}
		~Setting() noexcept
		{
			if (this->type == "Keyboard"		|| 
				this->type == "MouseButton"		|| 
				this->type == "MouseWheel"		|| 
				this->type == "JoystickButton"	|| 
				this->type == "JoystickAxis")
				this->value.m_input.~unique_ptr();
			else if (this->type == "string")
				this->value.m_string.~basic_string();
		};

		operator int& () noexcept { return this->value.m_int; };
		operator bool& () noexcept { return this->value.m_bool; };
		operator std::uint32_t& () noexcept { return this->value.m_u32; };
		operator std::uint64_t& () noexcept { return this->value.m_u64; };
		operator std::string& () noexcept { return this->value.m_string; };

		std::string_view type;
		Value value;
	};
public:
	~SettingsManager() noexcept = default;

	[[nodiscard]] auto& operator[](const std::string id) noexcept { return this->m_settings.at(id); };

	[[nodiscard]] static SettingsManager& getInstance(const char* path = "Settings.ini");
	[[nodiscard]] const bool save(const char* path) noexcept;
	[[nodiscard]] const bool load(const char* path) noexcept;
private:
	explicit SettingsManager(const char* path) noexcept;

	std::unordered_map<std::string, std::unordered_map<std::string, Setting>> m_settings;
	bool m_first = true;
};