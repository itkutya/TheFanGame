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

inline constexpr sf::Keyboard::Scancode StringToScanCode(const std::string& str) noexcept
{
	if (str == "A")							return sf::Keyboard::Scancode::A;
	else if (str == "Apostrophe")			return sf::Keyboard::Scancode::Apostrophe;
	else if (str == "Application")			return sf::Keyboard::Scancode::Application;		 
	else if (str == "B")					return sf::Keyboard::Scancode::B;					 
	else if (str == "Back")					return sf::Keyboard::Scancode::Back;				 
	else if (str == "Backslash")			return sf::Keyboard::Scancode::Backslash;			 
	else if (str == "Backspace")			return sf::Keyboard::Scancode::Backspace;			 
	else if (str == "C")					return sf::Keyboard::Scancode::C;					 
	else if (str == "CapsLock")				return sf::Keyboard::Scancode::CapsLock;			 
	else if (str == "Comma")				return sf::Keyboard::Scancode::Comma;				 
	else if (str == "Copy")					return sf::Keyboard::Scancode::Copy;				 
	else if (str == "Cut")					return sf::Keyboard::Scancode::Cut;				 
	else if (str == "D")					return sf::Keyboard::Scancode::D;		
	else if (str == "Delete")				return sf::Keyboard::Scancode::Delete;	
	else if (str == "Down")					return sf::Keyboard::Scancode::Down;			
	else if (str == "E")					return sf::Keyboard::Scancode::E;					 
	else if (str == "End")					return sf::Keyboard::Scancode::End;		
	else if (str == "Enter")				return sf::Keyboard::Scancode::Enter;	
	else if (str == "Equal")				return sf::Keyboard::Scancode::Equal;	
	else if (str == "Escape")				return sf::Keyboard::Scancode::Escape;	
	else if (str == "Execute")				return sf::Keyboard::Scancode::Execute;	
	else if (str == "F")					return sf::Keyboard::Scancode::F;		
	else if (str == "F1")					return sf::Keyboard::Scancode::F1;			
	else if (str == "F2")					return sf::Keyboard::Scancode::F2;			
	else if (str == "F3")					return sf::Keyboard::Scancode::F3;		
	else if (str == "F4")					return sf::Keyboard::Scancode::F4;	
	else if (str == "F5")					return sf::Keyboard::Scancode::F5;	
	else if (str == "F6")					return sf::Keyboard::Scancode::F6;	
	else if (str == "F7")					return sf::Keyboard::Scancode::F7;	
	else if (str == "F8")					return sf::Keyboard::Scancode::F8;	
	else if (str == "F9")					return sf::Keyboard::Scancode::F9;	
	else if (str == "F10")					return sf::Keyboard::Scancode::F10;	
	return sf::Keyboard::Scancode::F11;	
	else if (str == "F11")
	return sf::Keyboard::Scancode::F12;		
	else if (str == "F12")
	return sf::Keyboard::Scancode::F13;		
	else if (str == "F13")
	return sf::Keyboard::Scancode::F14;	
	else if (str == "F14")
	return sf::Keyboard::Scancode::F15;	
	else if (str == "F15")
	return sf::Keyboard::Scancode::F16;		
	else if (str == "F16")
	return sf::Keyboard::Scancode::F17;	
	else if (str == "F17")
	return sf::Keyboard::Scancode::F18;	
	else if (str == "F18")
	return sf::Keyboard::Scancode::F19;	
	else if (str == "F19")
	return sf::Keyboard::Scancode::F20;	
	else if (str == "F20")
	return sf::Keyboard::Scancode::F21;	
	else if (str == "F21")
	return sf::Keyboard::Scancode::F22;		
	else if (str == "F22")
	return sf::Keyboard::Scancode::F23;		
	else if (str == "F23")
	return sf::Keyboard::Scancode::F24;		
	else if (str == "F24")
	return sf::Keyboard::Scancode::Favorites;	
	else if (str == "Favorites")
	return sf::Keyboard::Scancode::Forward;		
	else if (str == "Forward")
	return sf::Keyboard::Scancode::G;			
	else if (str == "G")
	return sf::Keyboard::Scancode::Grave;		
	else if (str == "Grave")
	return sf::Keyboard::Scancode::H;			
	else if (str == "H")
	return sf::Keyboard::Scancode::Help;		
	else if (str == "Help")
	return sf::Keyboard::Scancode::Home;		
	else if (str == "Home")
	return sf::Keyboard::Scancode::HomePage;	
	else if (str == "HomePage")
	return sf::Keyboard::Scancode::Hyphen;		
	else if (str == "Hyphen")
	return sf::Keyboard::Scancode::I;			
	else if (str == "I")
	return sf::Keyboard::Scancode::Insert;		
	else if (str == "Insert")
	return sf::Keyboard::Scancode::J;			
	else if (str == "J")
	return sf::Keyboard::Scancode::K;			
	else if (str == "K")
	return sf::Keyboard::Scancode::L;			
	else if (str == "L")
	return sf::Keyboard::Scancode::LAlt;		
	else if (str == "LAlt")
	else if (str == "LaunchApplication1")		return sf::Keyboard::Scancode::LaunchApplication1; 
	else if (str == "LaunchApplication2")		return sf::Keyboard::Scancode::LaunchApplication2; 
	return sf::Keyboard::Scancode::LaunchMail;		
	else if (str == "LaunchMail")
	return sf::Keyboard::Scancode::LaunchMediaSelect;
	else if (str == "LaunchMediaSelect")
	return sf::Keyboard::Scancode::LBracket;		
	else if (str == "LBracket")
	return sf::Keyboard::Scancode::LControl;		
	else if (str == "LControl")
	return sf::Keyboard::Scancode::Left;			
	else if (str == "Left")
	return sf::Keyboard::Scancode::LShift;			
	else if (str == "LShift")
	return sf::Keyboard::Scancode::LSystem;			
	else if (str == "LSystem")
	return sf::Keyboard::Scancode::M;				
	else if (str == "M")
	return sf::Keyboard::Scancode::MediaNextTrack;	 
	else if (str == "MediaNextTrack")
	return sf::Keyboard::Scancode::MediaPlayPause;	
	else if (str == "MediaPlayPause")
	return sf::Keyboard::Scancode::MediaPreviousTrack; 
	else if (str == "MediaPreviousTrack")
	return sf::Keyboard::Scancode::MediaStop;		
	else if (str == "MediaStop")
	return sf::Keyboard::Scancode::Menu;			
	else if (str == "Menu")
	return sf::Keyboard::Scancode::ModeChange;		
	else if (str == "ModeChange")
	return sf::Keyboard::Scancode::N;				
	else if (str == "N")
	return sf::Keyboard::Scancode::NonUsBackslash;	
	else if (str == "NonUsBackslash")
	return sf::Keyboard::Scancode::Num0;			
	else if (str == "Num0")
	return sf::Keyboard::Scancode::Num1;			
	else if (str == "Num1")
	return sf::Keyboard::Scancode::Num2;			
	else if (str == "Num2")
	return sf::Keyboard::Scancode::Num3;			
	else if (str == "Num3")
	return sf::Keyboard::Scancode::Num4;		
	else if (str == "Num4")
	return sf::Keyboard::Scancode::Num5;		
	else if (str == "Num5")
	return sf::Keyboard::Scancode::Num6;		
	else if (str == "Num6")
	return sf::Keyboard::Scancode::Num7;		
	else if (str == "Num7")
	return sf::Keyboard::Scancode::Num8;		
	else if (str == "Num8")
	return sf::Keyboard::Scancode::Num9;		
	else if (str == "Num9")
	return sf::Keyboard::Scancode::NumLock;		
	else if (str == "NumLock")
	return sf::Keyboard::Scancode::Numpad0;		
	else if (str == "Numpad0")
	return sf::Keyboard::Scancode::Numpad1;		
	else if (str == "Numpad1")
	return sf::Keyboard::Scancode::Numpad2;		
	else if (str == "Numpad2")
	return sf::Keyboard::Scancode::Numpad3;		
	else if (str == "Numpad3")
	return sf::Keyboard::Scancode::Numpad4;		
	else if (str == "Numpad4")
	return sf::Keyboard::Scancode::Numpad5;		
	else if (str == "Numpad5")
	return sf::Keyboard::Scancode::Numpad6;		
	else if (str == "Numpad6")
	return sf::Keyboard::Scancode::Numpad7;		
	else if (str == "Numpad7")
	return sf::Keyboard::Scancode::Numpad8;		
	else if (str == "Numpad8")
	return sf::Keyboard::Scancode::Numpad9;		
	else if (str == "Numpad9")
	return sf::Keyboard::Scancode::NumpadDecimal;	
	else if (str == "NumpadDecimal")
	return sf::Keyboard::Scancode::NumpadDivide;	
	else if (str == "NumpadDivide")
	return sf::Keyboard::Scancode::NumpadEnter;		
	else if (str == "NumpadEnter")
	return sf::Keyboard::Scancode::NumpadEqual;	
	else if (str == "NumpadEqual")
	return sf::Keyboard::Scancode::NumpadMinus;	
	else if (str == "NumpadMinus")
	return sf::Keyboard::Scancode::NumpadMultiply;	
	else if (str == "NumpadMultiply")
	return sf::Keyboard::Scancode::NumpadPlus;		
	else if (str == "NumpadPlus")
	return sf::Keyboard::Scancode::O;			
	else if (str == "O")
	return sf::Keyboard::Scancode::P;			
	else if (str == "P")
	return sf::Keyboard::Scancode::PageDown;	
	else if (str == "PageDown")
	return sf::Keyboard::Scancode::PageUp;		
	else if (str == "PageUp")
	return sf::Keyboard::Scancode::Paste;		
	else if (str == "Paste")
	return sf::Keyboard::Scancode::Pause;		
	else if (str == "Pause")
	return sf::Keyboard::Scancode::Period;		
	else if (str == "Period")
	return sf::Keyboard::Scancode::PrintScreen;	
	else if (str == "PrintScreen")
	return sf::Keyboard::Scancode::Q;				
	else if (str == "Q")
	return sf::Keyboard::Scancode::R;			
	else if (str == "R")
	return sf::Keyboard::Scancode::RAlt;		
	else if (str == "RAlt")
	return sf::Keyboard::Scancode::RBracket;
	else if (str == "RBracket")
	return sf::Keyboard::Scancode::RControl;	
	else if (str == "RControl")
	return sf::Keyboard::Scancode::Redo;		
	else if (str == "Redo")
	return sf::Keyboard::Scancode::Refresh;		
	else if (str == "Refresh")
	return sf::Keyboard::Scancode::Right;		
	else if (str == "Right")
	return sf::Keyboard::Scancode::RShift;	
	else if (str == "RShift")
	return sf::Keyboard::Scancode::RSystem;		
	else if (str == "RSystem")
	return sf::Keyboard::Scancode::S;			
	else if (str == "S")
	//else if (str == "ScancodeCount")			return sf::Keyboard::Scancode::ScancodeCount;    
	return sf::Keyboard::Scancode::ScrollLock;		
	else if (str == "ScrollLock")
	return sf::Keyboard::Scancode::Search;		
	else if (str == "Search")
	return sf::Keyboard::Scancode::Select;		
	else if (str == "Select")
	return sf::Keyboard::Scancode::Semicolon;		
	else if (str == "Semicolon")
	return sf::Keyboard::Scancode::Slash;		
	else if (str == "Slash")
	return sf::Keyboard::Scancode::Space;	
	else if (str == "Space")
	return sf::Keyboard::Scancode::Stop;	
	else if (str == "Stop")
	return sf::Keyboard::Scancode::T;		
	else if (str == "T")
	return sf::Keyboard::Scancode::Tab;		
	else if (str == "Tab")
	return sf::Keyboard::Scancode::U;		
	else if (str == "U")
	return sf::Keyboard::Scancode::Undo;	
	else if (str == "Undo")
	//else if (str == "Unknown")				return sf::Keyboard::Scancode::Unknown;
	return sf::Keyboard::Scancode::Up;		
	else if (str == "Up")
	return sf::Keyboard::Scancode::V;		
	else if (str == "V")
	return sf::Keyboard::Scancode::VolumeDown;	
	else if (str == "VolumeDown")
	return sf::Keyboard::Scancode::VolumeMute;	
	else if (str == "VolumeMute")
	return sf::Keyboard::Scancode::VolumeUp;	
	else if (str == "VolumeUp")
	return sf::Keyboard::Scancode::W;			
	else if (str == "W")
	return sf::Keyboard::Scancode::X;		
	else if (str == "X")
	return sf::Keyboard::Scancode::Y;		
	else if (str == "Y")
	else if (str == "Z")			return sf::Keyboard::Scancode::Z;		
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