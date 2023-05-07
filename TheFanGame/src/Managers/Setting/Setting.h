#pragma once

#include <string>
#include <string_view>
#include <memory>

#include "Managers/Input/InputTypes.h"

struct Setting
{
	union Value
	{
		~Value() noexcept {};

		std::string& operator=(std::string rhs) noexcept { this->m_string = rhs; return this->m_string; };

		std::string m_string = "Null";
		std::unique_ptr<Input> m_input;
		int m_int;
		float m_float;
		bool m_bool;
		std::uint32_t m_u32;
		std::uint64_t m_u64;
	};

	Setting() noexcept : m_type("Null") {};
	Setting(const std::string t) noexcept : m_type(t)
	{
		if (this->m_type == "int")
			this->m_value.m_int = 0;
		else if (this->m_type == "float")
			this->m_value.m_float = 0.f;
		else if (this->m_type == "bool")
			this->m_value.m_bool = false;
		else if (this->m_type == "string")
			new (&this->m_value.m_string) std::string("Null");
		else if (this->m_type == "u32")
			this->m_value.m_u32 = 0;
		else if (this->m_type == "u64")
			this->m_value.m_u64 = 0;
		else if (this->m_type == "Keyboard")
			new (&this->m_value.m_input) std::unique_ptr<Keyboard>(std::make_unique<Keyboard>(sf::Keyboard::Scancode::A));
		else if (this->m_type == "MouseButton")
			new (&this->m_value.m_input) std::unique_ptr<MouseButton>(std::make_unique<MouseButton>(sf::Mouse::Button::Left));
		else if (this->m_type == "MouseWheel")
			new (&this->m_value.m_input) std::unique_ptr<MouseWheel>(std::make_unique<MouseWheel>(sf::Mouse::Wheel::VerticalWheel));
		else if (this->m_type == "JoystickButton")
			new (&this->m_value.m_input) std::unique_ptr<JoystickButton>(std::make_unique<JoystickButton>(0));
		else if (this->m_type == "JoystickAxis")
			new (&this->m_value.m_input) std::unique_ptr<JoystickAxis>(std::make_unique<JoystickAxis>(sf::Joystick::Axis::X));
	};
	Setting(const Setting& other) noexcept : m_type(other.m_type)
	{
		if (this->m_type == "int")
			this->m_value.m_int = other.m_value.m_int;
		else if (this->m_type == "float")
			this->m_value.m_float = other.m_value.m_float;
		else if (this->m_type == "bool")
			this->m_value.m_bool = other.m_value.m_bool;
		else if (this->m_type == "string")
			new (&this->m_value.m_string) std::string(other.m_value.m_string);
		else if (this->m_type == "u32")
			this->m_value.m_u32 = other.m_value.m_u32;
		else if (this->m_type == "u64")
			this->m_value.m_u64 = other.m_value.m_u64;
		else if (this->m_type == "Keyboard")
			new (&this->m_value.m_input) std::unique_ptr<Keyboard>(std::make_unique<Keyboard>(static_cast<Keyboard*>(other.m_value.m_input.get())->m_KeyCode));
		else if (this->m_type == "MouseButton")
			new (&this->m_value.m_input) std::unique_ptr<MouseButton>(std::make_unique<MouseButton>(static_cast<MouseButton*>(other.m_value.m_input.get())->m_MouseButton));
		else if (this->m_type == "MouseWheel")
			new (&this->m_value.m_input) std::unique_ptr<MouseWheel>(std::make_unique<MouseWheel>(static_cast<MouseWheel*>(other.m_value.m_input.get())->m_MouseWheel));
		else if (this->m_type == "JoystickButton")
			new (&this->m_value.m_input) std::unique_ptr<JoystickButton>(std::make_unique<JoystickButton>(static_cast<JoystickButton*>(other.m_value.m_input.get())->m_joystickButton));
		else if (this->m_type == "JoystickAxis")
			new (&this->m_value.m_input) std::unique_ptr<JoystickAxis>(std::make_unique<JoystickAxis>(static_cast<JoystickAxis*>(other.m_value.m_input.get())->m_JoystickAxis));
	}
	~Setting() noexcept
	{
		if (this->m_type == "Keyboard" ||
			this->m_type == "MouseButton" ||
			this->m_type == "MouseWheel" ||
			this->m_type == "JoystickButton" ||
			this->m_type == "JoystickAxis")
			this->m_value.m_input.~unique_ptr();
		else if (this->m_type == "string")
			this->m_value.m_string.~basic_string();
	};

	operator int&   () noexcept { return this->m_value.m_int;   };
	operator float& () noexcept { return this->m_value.m_float; };
	operator bool&  () noexcept { return this->m_value.m_bool;  };
	operator std::uint32_t& () noexcept { return this->m_value.m_u32;    };
	operator std::uint64_t& () noexcept { return this->m_value.m_u64;    };
	operator std::string&   () noexcept { return this->m_value.m_string; };

	const std::string toString()
	{
		if (this->m_type == "int")
			return std::to_string(this->m_value.m_int);
		else if (this->m_type == "float")
			return std::to_string(this->m_value.m_float);
		else if (this->m_type == "bool")
			return std::to_string(this->m_value.m_bool);
		else if (this->m_type == "string")
			return this->m_value.m_string;
		else if (this->m_type == "u32")
			return std::to_string(this->m_value.m_u32);
		else if (this->m_type == "u64")
			return std::to_string(this->m_value.m_u64);
		else if (this->m_type == "Keyboard")
			return ScanCodeToString(static_cast<Keyboard*>(this->m_value.m_input.get())->m_KeyCode);
		else if (this->m_type == "MouseButton")
			return MouseButtonToString(static_cast<MouseButton*>(this->m_value.m_input.get())->m_MouseButton);
		else if (this->m_type == "MouseWheel")
			return MouseWheelToString(static_cast<MouseWheel*>(this->m_value.m_input.get())->m_MouseWheel);
		else if (this->m_type == "JoystickButton")
			return JoystickButtonToString(static_cast<JoystickButton*>(this->m_value.m_input.get())->m_joystickButton);
		else if (this->m_type == "JoystickAxis")
			return JoystickAxisToString(static_cast<JoystickAxis*>(this->m_value.m_input.get())->m_JoystickAxis);
		throw "Unimplemented!";
	}

	void setValue(const std::string& value)
	{
		if (this->m_type == "int")
			this->m_value.m_int = std::stoi(value);
		else if (this->m_type == "float")
			this->m_value.m_float = std::stof(value);
		else if (this->m_type == "bool")
			this->m_value.m_bool = std::stoi(value);
		else if (this->m_type == "string")
			this->m_value.m_string = value;
		else if (this->m_type == "u32")
			this->m_value.m_u32 = std::stoul(value);
		else if (this->m_type == "u64")
			this->m_value.m_u64 = std::stoull(value);
		else if (this->m_type == "Keyboard")
			this->m_value.m_input = std::make_unique<Keyboard>(StringToScanCode(value));
		else if (this->m_type == "MouseButton")
			this->m_value.m_input = std::make_unique<MouseButton>(StringToMouseButton(value));
		else if (this->m_type == "MouseWheel")
			this->m_value.m_input = std::make_unique<MouseWheel>(StringToMouseWheel(value));
		else if (this->m_type == "JoystickButton")
			this->m_value.m_input = std::make_unique<JoystickButton>(StringToJoystickButton(value));
		else if (this->m_type == "JoystickAxis")
			this->m_value.m_input = std::make_unique<JoystickAxis>(StringToJoystickAxis(value));
	}

	std::string m_type;
	Value m_value;
};