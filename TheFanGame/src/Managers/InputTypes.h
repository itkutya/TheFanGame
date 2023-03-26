#pragma once

#include <string>
#include <any>

#include "SFML/Window.hpp"

enum class InputType
{
	None, Keyboard, MouseButton, MouseWheel, JoystickButton, JoystickAxis, Count
};

class Input
{
public:
	explicit Input() noexcept = default;
	virtual ~Input() noexcept = default;

	virtual const std::any input() const noexcept = 0;

	InputType m_type = InputType::None;
};

class Keyboard : public Input
{
public:
	explicit Keyboard(sf::Keyboard::Scancode key) noexcept : m_KeyCode(key) { this->m_type = InputType::Keyboard; };
	virtual ~Keyboard() noexcept = default;

	virtual const std::any input() const noexcept override { return sf::Keyboard::isKeyPressed(this->m_KeyCode); };

	sf::Keyboard::Scancode m_KeyCode = sf::Keyboard::Scancode();
};

class MouseButton : public Input
{
public:
	explicit MouseButton(sf::Mouse::Button button) noexcept : m_MouseButton(button) { this->m_type = InputType::MouseButton; };
	virtual ~MouseButton() noexcept = default;

	virtual const std::any input() const noexcept override { return sf::Mouse::isButtonPressed(this->m_MouseButton); };

	sf::Mouse::Button m_MouseButton = sf::Mouse::Button();
};

class MouseWheel : public Input
{
public:
	explicit MouseWheel(sf::Mouse::Wheel wheel) noexcept : m_MouseWheel(wheel) { this->m_type = InputType::MouseWheel; };
	virtual ~MouseWheel() noexcept = default;

	virtual const std::any input() const noexcept override { return 0.f; };

	sf::Mouse::Wheel m_MouseWheel = sf::Mouse::Wheel();
};

class JoystickButton : public Input
{
public:
	explicit JoystickButton(std::uint32_t button) noexcept : m_joystickButton(button) { this->m_type = InputType::JoystickButton; };
	virtual ~JoystickButton() noexcept = default;

	virtual const std::any input() const noexcept override { return sf::Joystick::isButtonPressed(0, this->m_joystickButton); };

	std::uint32_t m_joystickButton = 0;
};

class JoystickAxis : public Input
{
public:
	explicit JoystickAxis(sf::Joystick::Axis axis) noexcept : m_JoystickAxis(axis) { this->m_type = InputType::JoystickAxis; };
	virtual ~JoystickAxis() noexcept = default;

	virtual const std::any input() const noexcept override { return sf::Joystick::getAxisPosition(0, this->m_JoystickAxis); };

	sf::Joystick::Axis m_JoystickAxis = sf::Joystick::Axis();
};

inline sf::Keyboard::Scancode StringToScanCode(std::string& str) noexcept
{
	if (str == "A")
		return sf::Keyboard::Scancode::A;
	if (str == "B")
		return sf::Keyboard::Scancode::B;
	return sf::Keyboard::Scancode::D;
}

inline std::string ScanCodeToString(sf::Keyboard::Scancode code) noexcept
{
	switch (code)
	{
	case sf::Keyboard::Scancode::A:
		return "A";
	case sf::Keyboard::Scancode::B:
		return "B";
	}
	return "";
}

inline std::string ScanCodeToString(sf::Mouse::Button code) noexcept
{
	switch (code)
	{
	case sf::Mouse::Button::Left:
		return "Left";
	}
	return "";
}

inline std::string ScanCodeToString(sf::Mouse::Wheel code) noexcept
{
	switch (code)
	{
	case sf::Mouse::Wheel::VerticalWheel:
		return "VerticalWheel";
	case sf::Mouse::Wheel::HorizontalWheel:
		return "VerticalWheel";
	}
	return "";
}

inline std::string ScanCodeToString(std::uint32_t code) noexcept { return std::to_string(code); }

inline std::string ScanCodeToString(sf::Joystick::Axis code) noexcept
{
	switch (code)
	{
	case sf::Joystick::Axis::X:
		return "X";
	}
	return "";
}