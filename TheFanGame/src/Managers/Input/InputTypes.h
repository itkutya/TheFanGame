#pragma once

#include <cstdint>
#include <string>

#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Window/Joystick.hpp"

enum class InputType
{
	None, Keyboard, MouseButton, MouseWheel, JoystickButton, JoystickAxis, Count
};

struct Input 
{
	union InputReturnType
	{
		operator bool()  const noexcept { return this->m_bool;  };
		operator float() const noexcept { return this->m_float; };

		const bool operator>(float rhs)  const noexcept { return this->m_float >  rhs  ? true : false; };
		const bool operator<(float rhs)  const noexcept { return this->m_float <  rhs  ? true : false; };
		const bool operator==(float rhs) const noexcept { return this->m_float == rhs  ? true : false; };
		const bool operator!=(float rhs) const noexcept { return this->m_float != rhs  ? true : false; };

		bool  m_bool = false;
		float m_float;
	};

	[[noreturn]] virtual const InputReturnType input() const						{ throw "Unimplemented"; };
	[[noreturn]] virtual const InputReturnType input(sf::Event& event) const		{ throw "Unimplemented"; };
	[[noreturn]] virtual const InputReturnType input(const std::uint32_t id) const	{ throw "Unimplemented"; };

	InputType m_type = InputType::None;
	const bool m_changeable = true;
};

struct Keyboard : public Input
{
	Keyboard(sf::Keyboard::Scancode key) noexcept : m_KeyCode(key) { this->m_type = InputType::Keyboard; };
	
	const InputReturnType input() const override
	{
		InputReturnType irt;
		irt.m_bool = sf::Keyboard::isKeyPressed(this->m_KeyCode);
		return irt;
	};

	sf::Keyboard::Scancode m_KeyCode = sf::Keyboard::Scancode();
};

struct MouseButton : public Input
{
	MouseButton(sf::Mouse::Button button) noexcept : m_MouseButton(button) { this->m_type = InputType::MouseButton; };
	
	const InputReturnType input() const override 
	{
		InputReturnType irt;
		irt.m_bool = sf::Mouse::isButtonPressed(this->m_MouseButton);
		return irt;
	};

	sf::Mouse::Button m_MouseButton = sf::Mouse::Button();
};

struct MouseWheel : public Input
{
	MouseWheel(sf::Mouse::Wheel wheel) noexcept : m_MouseWheel(wheel) { this->m_type = InputType::MouseWheel; };
	
	const InputReturnType input(sf::Event& event) const override
	{
		InputReturnType irt;
		irt.m_float = event.mouseWheelScroll.wheel == this->m_MouseWheel ? event.mouseWheelScroll.delta : 0.f;
		return irt;
	};
	
	sf::Mouse::Wheel m_MouseWheel = sf::Mouse::Wheel();
};

struct JoystickButton : public Input
{
	JoystickButton(std::uint32_t button) noexcept : m_joystickButton(button) { this->m_type = InputType::JoystickButton; };
	
	const InputReturnType input(const std::uint32_t id) const override
	{
		InputReturnType irt;
		irt.m_bool = sf::Joystick::isButtonPressed(id, this->m_joystickButton);
		return irt;
	};
	
	std::uint32_t m_joystickButton = 0;
};

struct JoystickAxis : public Input
{
	JoystickAxis(sf::Joystick::Axis axis) noexcept : m_JoystickAxis(axis) { this->m_type = InputType::JoystickAxis; };
	
	const InputReturnType input(const std::uint32_t id) const override
	{
		InputReturnType irt;
		irt.m_float = sf::Joystick::getAxisPosition(id, this->m_JoystickAxis);
		return irt;
	};
	
	sf::Joystick::Axis m_JoystickAxis = sf::Joystick::Axis();
};

inline constexpr sf::Keyboard::Scancode StringToScanCode(const std::string& str)
{
	if (str == "A")						return sf::Keyboard::Scancode::A;
	if (str == "Apostrophe")			return sf::Keyboard::Scancode::Apostrophe;
	if (str == "Application")			return sf::Keyboard::Scancode::Application;
	if (str == "B")						return sf::Keyboard::Scancode::B;
	if (str == "Back")					return sf::Keyboard::Scancode::Back;
	if (str == "Backslash")				return sf::Keyboard::Scancode::Backslash;
	if (str == "Backspace")				return sf::Keyboard::Scancode::Backspace;
	if (str == "C")						return sf::Keyboard::Scancode::C;
	if (str == "CapsLock")				return sf::Keyboard::Scancode::CapsLock;
	if (str == "Comma")					return sf::Keyboard::Scancode::Comma;
	if (str == "Copy")					return sf::Keyboard::Scancode::Copy;
	if (str == "Cut")					return sf::Keyboard::Scancode::Cut;
	if (str == "D")						return sf::Keyboard::Scancode::D;
	if (str == "Delete")				return sf::Keyboard::Scancode::Delete;
	if (str == "Down")					return sf::Keyboard::Scancode::Down;
	if (str == "E")						return sf::Keyboard::Scancode::E;
	if (str == "End")					return sf::Keyboard::Scancode::End;
	if (str == "Enter")					return sf::Keyboard::Scancode::Enter;
	if (str == "Equal")					return sf::Keyboard::Scancode::Equal;
	if (str == "Escape")				return sf::Keyboard::Scancode::Escape;
	if (str == "Execute")				return sf::Keyboard::Scancode::Execute;
	if (str == "F")						return sf::Keyboard::Scancode::F;
	if (str == "F1")					return sf::Keyboard::Scancode::F1;
	if (str == "F2")					return sf::Keyboard::Scancode::F2;
	if (str == "F3")					return sf::Keyboard::Scancode::F3;
	if (str == "F4")					return sf::Keyboard::Scancode::F4;
	if (str == "F5")					return sf::Keyboard::Scancode::F5;
	if (str == "F6")					return sf::Keyboard::Scancode::F6;
	if (str == "F7")					return sf::Keyboard::Scancode::F7;
	if (str == "F8")					return sf::Keyboard::Scancode::F8;
	if (str == "F9")					return sf::Keyboard::Scancode::F9;
	if (str == "F10")					return sf::Keyboard::Scancode::F10;
	if (str == "F11")					return sf::Keyboard::Scancode::F11;
	if (str == "F12")					return sf::Keyboard::Scancode::F12;
	if (str == "F13")					return sf::Keyboard::Scancode::F13;
	if (str == "F14")					return sf::Keyboard::Scancode::F14;
	if (str == "F15")					return sf::Keyboard::Scancode::F15;
	if (str == "F16")					return sf::Keyboard::Scancode::F16;
	if (str == "F17")					return sf::Keyboard::Scancode::F17;
	if (str == "F18")					return sf::Keyboard::Scancode::F18;
	if (str == "F19")					return sf::Keyboard::Scancode::F19;
	if (str == "F20")					return sf::Keyboard::Scancode::F20;
	if (str == "F21")					return sf::Keyboard::Scancode::F21;
	if (str == "F22")					return sf::Keyboard::Scancode::F22;
	if (str == "F23")					return sf::Keyboard::Scancode::F23;
	if (str == "F24")					return sf::Keyboard::Scancode::F24;
	if (str == "Favorites")				return sf::Keyboard::Scancode::Favorites;
	if (str == "Forward")				return sf::Keyboard::Scancode::Forward;
	if (str == "G")						return sf::Keyboard::Scancode::G;
	if (str == "Grave")					return sf::Keyboard::Scancode::Grave;
	if (str == "H")						return sf::Keyboard::Scancode::H;
	if (str == "Help")					return sf::Keyboard::Scancode::Help;
	if (str == "Home")					return sf::Keyboard::Scancode::Home;
	if (str == "HomePage")				return sf::Keyboard::Scancode::HomePage;
	if (str == "Hyphen")				return sf::Keyboard::Scancode::Hyphen;
	if (str == "I")						return sf::Keyboard::Scancode::I;
	if (str == "Insert")				return sf::Keyboard::Scancode::Insert;
	if (str == "J")						return sf::Keyboard::Scancode::J;
	if (str == "K")						return sf::Keyboard::Scancode::K;
	if (str == "L")						return sf::Keyboard::Scancode::L;
	if (str == "LAlt")					return sf::Keyboard::Scancode::LAlt;
	if (str == "LaunchApplication1")	return sf::Keyboard::Scancode::LaunchApplication1;
	if (str == "LaunchApplication2")	return sf::Keyboard::Scancode::LaunchApplication2;
	if (str == "LaunchMail")			return sf::Keyboard::Scancode::LaunchMail;
	if (str == "LaunchMediaSelect")		return sf::Keyboard::Scancode::LaunchMediaSelect;
	if (str == "LBracket")				return sf::Keyboard::Scancode::LBracket;
	if (str == "LControl")				return sf::Keyboard::Scancode::LControl;
	if (str == "Left")					return sf::Keyboard::Scancode::Left;
	if (str == "LShift")				return sf::Keyboard::Scancode::LShift;
	if (str == "LSystem")				return sf::Keyboard::Scancode::LSystem;
	if (str == "M")						return sf::Keyboard::Scancode::M;
	if (str == "MediaNextTrack")		return sf::Keyboard::Scancode::MediaNextTrack;
	if (str == "MediaPlayPause")		return sf::Keyboard::Scancode::MediaPlayPause;
	if (str == "MediaPreviousTrack")	return sf::Keyboard::Scancode::MediaPreviousTrack;
	if (str == "MediaStop")				return sf::Keyboard::Scancode::MediaStop;
	if (str == "Menu")					return sf::Keyboard::Scancode::Menu;
	if (str == "ModeChange")			return sf::Keyboard::Scancode::ModeChange;
	if (str == "N")						return sf::Keyboard::Scancode::N;
	if (str == "NonUsBackslash")		return sf::Keyboard::Scancode::NonUsBackslash;
	if (str == "Num0")					return sf::Keyboard::Scancode::Num0;
	if (str == "Num1")					return sf::Keyboard::Scancode::Num1;
	if (str == "Num2")					return sf::Keyboard::Scancode::Num2;
	if (str == "Num3")					return sf::Keyboard::Scancode::Num3;
	if (str == "Num4")					return sf::Keyboard::Scancode::Num4;
	if (str == "Num5")					return sf::Keyboard::Scancode::Num5;
	if (str == "Num6")					return sf::Keyboard::Scancode::Num6;
	if (str == "Num7")					return sf::Keyboard::Scancode::Num7;
	if (str == "Num8")					return sf::Keyboard::Scancode::Num8;
	if (str == "Num9")					return sf::Keyboard::Scancode::Num9;
	if (str == "NumLock")				return sf::Keyboard::Scancode::NumLock;
	if (str == "Numpad0")				return sf::Keyboard::Scancode::Numpad0;
	if (str == "Numpad1")				return sf::Keyboard::Scancode::Numpad1;
	if (str == "Numpad2")				return sf::Keyboard::Scancode::Numpad2;
	if (str == "Numpad3")				return sf::Keyboard::Scancode::Numpad3;
	if (str == "Numpad4")				return sf::Keyboard::Scancode::Numpad4;
	if (str == "Numpad5")				return sf::Keyboard::Scancode::Numpad5;
	if (str == "Numpad6")				return sf::Keyboard::Scancode::Numpad6;
	if (str == "Numpad7")				return sf::Keyboard::Scancode::Numpad7;
	if (str == "Numpad8")				return sf::Keyboard::Scancode::Numpad8;
	if (str == "Numpad9")				return sf::Keyboard::Scancode::Numpad9;
	if (str == "NumpadDecimal")			return sf::Keyboard::Scancode::NumpadDecimal;
	if (str == "NumpadDivide")			return sf::Keyboard::Scancode::NumpadDivide;
	if (str == "NumpadEnter")			return sf::Keyboard::Scancode::NumpadEnter;
	if (str == "NumpadEqual")			return sf::Keyboard::Scancode::NumpadEqual;
	if (str == "NumpadMinus")			return sf::Keyboard::Scancode::NumpadMinus;
	if (str == "NumpadMultiply")		return sf::Keyboard::Scancode::NumpadMultiply;
	if (str == "NumpadPlus")			return sf::Keyboard::Scancode::NumpadPlus;
	if (str == "O")						return sf::Keyboard::Scancode::O;
	if (str == "P")						return sf::Keyboard::Scancode::P;
	if (str == "PageDown")				return sf::Keyboard::Scancode::PageDown;
	if (str == "PageUp")				return sf::Keyboard::Scancode::PageUp;
	if (str == "Paste")					return sf::Keyboard::Scancode::Paste;
	if (str == "Pause")					return sf::Keyboard::Scancode::Pause;
	if (str == "Period")				return sf::Keyboard::Scancode::Period;
	if (str == "PrintScreen")			return sf::Keyboard::Scancode::PrintScreen;
	if (str == "Q")						return sf::Keyboard::Scancode::Q;
	if (str == "R")						return sf::Keyboard::Scancode::R;
	if (str == "RAlt")					return sf::Keyboard::Scancode::RAlt;
	if (str == "RBracket")				return sf::Keyboard::Scancode::RBracket;
	if (str == "RControl")				return sf::Keyboard::Scancode::RControl;
	if (str == "Redo")					return sf::Keyboard::Scancode::Redo;
	if (str == "Refresh")				return sf::Keyboard::Scancode::Refresh;
	if (str == "Right")					return sf::Keyboard::Scancode::Right;
	if (str == "RShift")				return sf::Keyboard::Scancode::RShift;
	if (str == "RSystem")				return sf::Keyboard::Scancode::RSystem;
	if (str == "S")						return sf::Keyboard::Scancode::S;
	//if (str == "ScancodeCount")		return sf::Keyboard::Scancode::ScancodeCount;    
	if (str == "ScrollLock")			return sf::Keyboard::Scancode::ScrollLock;
	if (str == "Search")				return sf::Keyboard::Scancode::Search;
	if (str == "Select")				return sf::Keyboard::Scancode::Select;
	if (str == "Semicolon")				return sf::Keyboard::Scancode::Semicolon;
	if (str == "Slash")					return sf::Keyboard::Scancode::Slash;
	if (str == "Space")					return sf::Keyboard::Scancode::Space;
	if (str == "Stop")					return sf::Keyboard::Scancode::Stop;
	if (str == "T")						return sf::Keyboard::Scancode::T;
	if (str == "Tab")					return sf::Keyboard::Scancode::Tab;
	if (str == "U")						return sf::Keyboard::Scancode::U;
	if (str == "Undo")					return sf::Keyboard::Scancode::Undo;
	//if (str == "Unknown")				return sf::Keyboard::Scancode::Unknown;
	if (str == "Up")					return sf::Keyboard::Scancode::Up;
	if (str == "V")						return sf::Keyboard::Scancode::V;
	if (str == "VolumeDown")			return sf::Keyboard::Scancode::VolumeDown;
	if (str == "VolumeMute")			return sf::Keyboard::Scancode::VolumeMute;
	if (str == "VolumeUp")				return sf::Keyboard::Scancode::VolumeUp;
	if (str == "W")						return sf::Keyboard::Scancode::W;
	if (str == "X")						return sf::Keyboard::Scancode::X;
	if (str == "Y")						return sf::Keyboard::Scancode::Y;
	if (str == "Z")						return sf::Keyboard::Scancode::Z;
	throw "Unimplemented";
}

inline constexpr sf::Mouse::Button StringToMouseButton(const std::string& str)
{
	if (str == "Left")			return sf::Mouse::Button::Left;			
	if (str == "Right")			return sf::Mouse::Button::Right;
	if (str == "Middle")		return sf::Mouse::Button::Middle;	
	if (str == "XButton1")		return sf::Mouse::Button::XButton1;
	if (str == "XButton2")		return sf::Mouse::Button::XButton2;
	//if (str == "ButtonCount")	return sf::Mouse::Button::ButtonCount;	
	throw "Unimplemented";
}

inline constexpr sf::Mouse::Wheel StringToMouseWheel(const std::string& str)
{
	if (str == "HorizontalWheel")	return sf::Mouse::Wheel::HorizontalWheel;
	if (str == "VerticalWheel")		return sf::Mouse::Wheel::VerticalWheel;
	throw "Unimplemented";
}

inline const std::uint32_t StringToJoystickButton(const std::string& str) { return std::stoul(str); }

inline constexpr sf::Joystick::Axis StringToJoystickAxis(const std::string& str)
{
	if (str == "X")		return sf::Joystick::Axis::X;
	if (str == "Y")		return sf::Joystick::Axis::Y;	
	if (str == "Z")		return sf::Joystick::Axis::Z;
	if (str == "R")		return sf::Joystick::Axis::R;
	if (str == "U")		return sf::Joystick::Axis::U;
	if (str == "V")		return sf::Joystick::Axis::V;	
	if (str == "PovX")	return sf::Joystick::Axis::PovX;	 
	if (str == "PovY")	return sf::Joystick::Axis::PovY;	 
	throw "Unimplemented";
}

inline constexpr const std::string ScanCodeToString(const sf::Keyboard::Scancode code)
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
	throw "Unimplemented";
}

inline constexpr const std::string MouseButtonToString(const sf::Mouse::Button code)
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
	throw "Unimplemented";
}

inline constexpr const std::string MouseWheelToString(const sf::Mouse::Wheel code)
{
	switch (code)
	{
	case sf::Mouse::Wheel::VerticalWheel:	return "VerticalWheel";
	case sf::Mouse::Wheel::HorizontalWheel:	return "HorizotnalWheel";
	}
	throw "Unimplemented";
}

inline const std::string JoystickButtonToString(const std::uint32_t code)
{
	return std::to_string(code);
}

inline constexpr const std::string JoystickAxisToString(const sf::Joystick::Axis code)
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
	throw "Unimplemented";
}