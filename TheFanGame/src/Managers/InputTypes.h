#pragma once

#include <string>
#include <any>

#include "SFML/Window.hpp"

namespace std
{
	[[noreturn]] inline void unreachable()
	{
#ifdef __GNUC__
		__builtin_unreachable();
#elifdef _MSC_VER
		__assume(false);
#endif
	};
};

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

inline constexpr sf::Keyboard::Scancode StringToScanCode(std::string& str) noexcept
{
	if (str == "A")
		return sf::Keyboard::Scancode::A;
	if (str == "B")
		return sf::Keyboard::Scancode::B;
	return sf::Keyboard::Scancode::D;
}

inline constexpr std::string ScanCodeToString(const sf::Keyboard::Scancode code) noexcept
{
	switch (code)
	{
	case sf::Keyboard::Scancode::A:					 return "A";
	case sf::Keyboard::Scancode::Apostrophe:		 return "Apostrophe";
	case sf::Keyboard::Scancode::Application:		 return "Application";
	case sf::Keyboard::Scancode::B:					 return "B";
	case sf::Keyboard::Scancode::Back:				 return "Back";
	case sf::Keyboard::Scancode::Backslash:			 return "Backslash";
	case sf::Keyboard::Scancode::Backspace:			 return "Backspace";
	case sf::Keyboard::Scancode::C:					 return "C";
	case sf::Keyboard::Scancode::CapsLock:			 return "CapsLock";
	case sf::Keyboard::Scancode::Comma:				 return "Comma";
	case sf::Keyboard::Scancode::Copy:				 return "Copy";
	case sf::Keyboard::Scancode::Cut:				 return "Cut";
	case sf::Keyboard::Scancode::D:					 return "D";
	case sf::Keyboard::Scancode::Delete:			 return "Delete";
	case sf::Keyboard::Scancode::Down:				 return "Down";
	case sf::Keyboard::Scancode::E:					 return "E";
	case sf::Keyboard::Scancode::End:				 return "End";
	case sf::Keyboard::Scancode::Enter:				 return "Enter";
	case sf::Keyboard::Scancode::Equal:				 return "Equal";
	case sf::Keyboard::Scancode::Escape:			 return "Escape";
	case sf::Keyboard::Scancode::Execute:			 return "Execute";
	case sf::Keyboard::Scancode::F:					 return "F";
	case sf::Keyboard::Scancode::F1:				 return "F1";
	case sf::Keyboard::Scancode::F2:				 return "F2";
	case sf::Keyboard::Scancode::F3:				 return "F3";
	case sf::Keyboard::Scancode::F4:				 return "F4";
	case sf::Keyboard::Scancode::F5:				 return "F5";
	case sf::Keyboard::Scancode::F6:				 return "F6";
	case sf::Keyboard::Scancode::F7:				 return "F7";
	case sf::Keyboard::Scancode::F8:				 return "F8";
	case sf::Keyboard::Scancode::F9:				 return "F9";
	case sf::Keyboard::Scancode::F10:				 return "F10";
	case sf::Keyboard::Scancode::F11:				 return "F11";
	case sf::Keyboard::Scancode::F12:				 return "F12";
	case sf::Keyboard::Scancode::F13:				 return "F13";
	case sf::Keyboard::Scancode::F14:				 return "F14";
	case sf::Keyboard::Scancode::F15:				 return "F15";
	case sf::Keyboard::Scancode::F16:				 return "F16";
	case sf::Keyboard::Scancode::F17:				 return "F17";
	case sf::Keyboard::Scancode::F18:				 return "F18";
	case sf::Keyboard::Scancode::F19:				 return "F19";
	case sf::Keyboard::Scancode::F20:				 return "F20";
	case sf::Keyboard::Scancode::F21:				 return "F21";
	case sf::Keyboard::Scancode::F22:				 return "F22";
	case sf::Keyboard::Scancode::F23:				 return "F23";
	case sf::Keyboard::Scancode::F24:				 return "F24";
	case sf::Keyboard::Scancode::Favorites:			 return "Favorites";
	case sf::Keyboard::Scancode::Forward:			 return "Forward";
	case sf::Keyboard::Scancode::G:					 return "G";
	case sf::Keyboard::Scancode::Grave:				 return "Grave";
	case sf::Keyboard::Scancode::H:					 return "H";
	case sf::Keyboard::Scancode::Help:				 return "Help";
	case sf::Keyboard::Scancode::Home:				 return "Home";
	case sf::Keyboard::Scancode::HomePage:			 return "HomePage";
	case sf::Keyboard::Scancode::Hyphen:			 return "Hyphen";
	case sf::Keyboard::Scancode::I:					 return "I";
	case sf::Keyboard::Scancode::Insert:			 return "Insert";
	case sf::Keyboard::Scancode::J:					 return "J";
	case sf::Keyboard::Scancode::K:					 return "K";
	case sf::Keyboard::Scancode::L:					 return "L";
	case sf::Keyboard::Scancode::LAlt:				 return "LAlt";
	case sf::Keyboard::Scancode::LaunchApplication1: return "LaunchApplication1";
	case sf::Keyboard::Scancode::LaunchApplication2: return "LaunchApplication2";
	case sf::Keyboard::Scancode::LaunchMail:		 return "LaunchMail";
	case sf::Keyboard::Scancode::LaunchMediaSelect:	 return "LaunchMediaSelect";
	case sf::Keyboard::Scancode::LBracket:			 return "LBracket";
	case sf::Keyboard::Scancode::LControl:			 return "LControl";
	case sf::Keyboard::Scancode::Left:				 return "Left";
	case sf::Keyboard::Scancode::LShift:			 return "LShift";
	case sf::Keyboard::Scancode::LSystem:			 return "LSystem";
	case sf::Keyboard::Scancode::M:					 return "M";
	case sf::Keyboard::Scancode::MediaNextTrack:	 return "MediaNextTrack";
	case sf::Keyboard::Scancode::MediaPlayPause:	 return "MediaPlayPause";
	case sf::Keyboard::Scancode::MediaPreviousTrack: return "MediaPreviousTrack";
	case sf::Keyboard::Scancode::MediaStop:			 return "MediaStop";
	case sf::Keyboard::Scancode::Menu:				 return "Menu";
	case sf::Keyboard::Scancode::ModeChange:		 return "ModeChange";
	case sf::Keyboard::Scancode::N:					 return "N";
	case sf::Keyboard::Scancode::NonUsBackslash:	 return "NonUsBackslash";
	case sf::Keyboard::Scancode::Num0:				 return "Num0";
	case sf::Keyboard::Scancode::Num1:				 return "Num1";
	case sf::Keyboard::Scancode::Num2:				 return "Num2";
	case sf::Keyboard::Scancode::Num3:				 return "Num3";
	case sf::Keyboard::Scancode::Num4:				 return "Num4";
	case sf::Keyboard::Scancode::Num5:				 return "Num5";
	case sf::Keyboard::Scancode::Num6:				 return "Num6";
	case sf::Keyboard::Scancode::Num7:				 return "Num7";
	case sf::Keyboard::Scancode::Num8:				 return "Num8";
	case sf::Keyboard::Scancode::Num9:				 return "Num9";
	case sf::Keyboard::Scancode::NumLock:			 return "NumLock";
	case sf::Keyboard::Scancode::Numpad0:			 return "Numpad0";
	case sf::Keyboard::Scancode::Numpad1:			 return "Numpad1";
	case sf::Keyboard::Scancode::Numpad2:			 return "Numpad2";
	case sf::Keyboard::Scancode::Numpad3:			 return "Numpad3";
	case sf::Keyboard::Scancode::Numpad4:			 return "Numpad4";
	case sf::Keyboard::Scancode::Numpad5:			 return "Numpad5";
	case sf::Keyboard::Scancode::Numpad6:			 return "Numpad6";
	case sf::Keyboard::Scancode::Numpad7:			 return "Numpad7";
	case sf::Keyboard::Scancode::Numpad8:			 return "Numpad8";
	case sf::Keyboard::Scancode::Numpad9:			 return "Numpad9";
	case sf::Keyboard::Scancode::NumpadDecimal:		 return "NumpadDecimal";
	case sf::Keyboard::Scancode::NumpadDivide:		 return "NumpadDivide";
	case sf::Keyboard::Scancode::NumpadEnter:		 return "NumpadEnter";
	case sf::Keyboard::Scancode::NumpadEqual:		 return "NumpadEqual";
	case sf::Keyboard::Scancode::NumpadMinus:		 return "NumpadMinus";
	case sf::Keyboard::Scancode::NumpadMultiply:	 return "NumpadMultiply";
	case sf::Keyboard::Scancode::NumpadPlus:		 return "NumpadPlus";
	case sf::Keyboard::Scancode::O:					 return "O";
	case sf::Keyboard::Scancode::P:				     return "P";
	case sf::Keyboard::Scancode::PageDown:			 return "PageDown";
	case sf::Keyboard::Scancode::PageUp:			 return "PageUp";
	case sf::Keyboard::Scancode::Paste:				 return "Paste";
	case sf::Keyboard::Scancode::Pause:				 return "Pause";
	case sf::Keyboard::Scancode::Period:			 return "Period";
	case sf::Keyboard::Scancode::PrintScreen:		 return "PrintScreen";
	case sf::Keyboard::Scancode::Q:					 return "Q";
	case sf::Keyboard::Scancode::R:					 return "R";
	case sf::Keyboard::Scancode::RAlt:				 return "RAlt";
	case sf::Keyboard::Scancode::RBracket:			 return "RBracket";
	case sf::Keyboard::Scancode::RControl:			 return "RControl";
	case sf::Keyboard::Scancode::Redo:				 return "Redo";
	case sf::Keyboard::Scancode::Refresh:			 return "Refresh";
	case sf::Keyboard::Scancode::Right:				 return "Right";
	case sf::Keyboard::Scancode::RShift:			 return "RShift";
	case sf::Keyboard::Scancode::RSystem:			 return "RSystem";
	case sf::Keyboard::Scancode::S:					 return "S";
	//case sf::Keyboard::Scancode::ScancodeCount:    return "ScancodeCount";
	case sf::Keyboard::Scancode::ScrollLock:		 return "ScrollLock";
	case sf::Keyboard::Scancode::Search:			 return "Search";
	case sf::Keyboard::Scancode::Select:			 return "Select";
	case sf::Keyboard::Scancode::Semicolon:			 return "Semicolon";
	case sf::Keyboard::Scancode::Slash:				 return "Slash";
	case sf::Keyboard::Scancode::Space:				 return "Space";
	case sf::Keyboard::Scancode::Stop:				 return "Stop";
	case sf::Keyboard::Scancode::T:					 return "T";
	case sf::Keyboard::Scancode::Tab:				 return "Tab";
	case sf::Keyboard::Scancode::U:					 return "U";
	case sf::Keyboard::Scancode::Undo:				 return "Undo";
	//case sf::Keyboard::Scancode::Unknown:			 return "Unknown";
	case sf::Keyboard::Scancode::Up:				 return "Up";
	case sf::Keyboard::Scancode::V:					 return "V";
	case sf::Keyboard::Scancode::VolumeDown:		 return "VolumeDown";
	case sf::Keyboard::Scancode::VolumeMute:		 return "VolumeMute";
	case sf::Keyboard::Scancode::VolumeUp:			 return "VolumeUp";
	case sf::Keyboard::Scancode::W:					 return "W";
	case sf::Keyboard::Scancode::X:					 return "X";
	case sf::Keyboard::Scancode::Y:					 return "Y";
	case sf::Keyboard::Scancode::Z:					 return "Z";
	}
	std::unreachable();
}

inline constexpr std::string ScanCodeToString(const sf::Mouse::Button code) noexcept
{
	switch (code)
	{
	case sf::Mouse::Button::Left:			return "Left";
	case sf::Mouse::Button::Right:			return "Right";
	case sf::Mouse::Button::Middle:			return "Middle";
	case sf::Mouse::Button::XButton1:		return "XButton1";
	case sf::Mouse::Button::XButton2:		return "XButton2";
	//case sf::Mouse::Button::ButtonCount:	return "ButtonCount";
	}
	std::unreachable();
}

inline constexpr std::string ScanCodeToString(const sf::Mouse::Wheel code) noexcept
{
	switch (code)
	{
	case sf::Mouse::Wheel::VerticalWheel:	return "VerticalWheel";
	case sf::Mouse::Wheel::HorizontalWheel:	return "HorizotnalWheel";
	}
	std::unreachable();
}

inline const std::string ScanCodeToString(const std::uint32_t code) noexcept { return std::to_string(code); }

inline constexpr std::string ScanCodeToString(const sf::Joystick::Axis code) noexcept
{
	switch (code)
	{
	case sf::Joystick::Axis::X:		return "X";
	case sf::Joystick::Axis::Y:		return "Y";
	case sf::Joystick::Axis::Z:		return "Z";
	case sf::Joystick::Axis::R:		return "R";
	case sf::Joystick::Axis::U:		return "U";
	case sf::Joystick::Axis::V:		return "V";
	case sf::Joystick::Axis::PovX:	return "PovX";
	case sf::Joystick::Axis::PovY:	return "PovY";
	}
	std::unreachable();
}