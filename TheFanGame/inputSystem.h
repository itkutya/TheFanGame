#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <array>
#include <vector>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Window.hpp"
		#include "SFML64/Graphics.hpp"
	#else
		#include "SFML32/Window.hpp"
		#include "SFML32/Graphics.hpp"
	#endif
#endif

enum class InputType
{
	KeyboardInput,
	MouseInput,
	JoystickButtonInput,
	JoystickAxisInput
};
struct m_Keys
{
	InputType m_InputType;
	sf::Event::EventType m_EventType;
	sf::Keyboard::Key m_KeyCode;
	sf::Mouse::Button m_MouseButton;
	sf::Joystick::Axis m_JoystickAxis;
	std::uint32_t m_joystickButton;
};

class inputSystem
{
public:
	inputSystem() = delete;
	inputSystem(const inputSystem&) = delete;
	inputSystem(const inputSystem&&) = delete;
	virtual ~inputSystem() {};

	static const void init();
	static const bool input(m_Keys& key, sf::Event* e = nullptr) noexcept;
	static const char* convert(const m_Keys& it);
	static const char* PressOrRelease(const m_Keys& it);
	static const void saveInput(const std::pair<std::string, m_Keys>& temp) noexcept;
	static const void clear() noexcept;
	
	static std::unordered_map<std::string, m_Keys> m_Action;
private:
	static bool isJoystickConnected;
};