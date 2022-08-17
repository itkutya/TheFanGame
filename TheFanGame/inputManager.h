#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Window/Event.hpp"
	#else
		#include "SFML32/Window/Event.hpp"
	#endif
#endif

enum class InputType
{
	KeyboardInput,
	MouseInput,
	JoystickInput
};
struct m_Keys
{
	InputType m_InputType;
	sf::Event::EventType m_EventType;
	sf::Keyboard::Key m_KeyCode;
	sf::Mouse::Button m_MouseButton;
};

class inputManager
{
public:
	inputManager() = delete;
	inputManager(const inputManager&) = delete;
	inputManager(const inputManager&&) = delete;
	virtual ~inputManager() {};

	static const void init();
	static const bool input(m_Keys& key, sf::Event* e = nullptr) noexcept;
	
	static std::unordered_map<std::string, m_Keys> m_Action;
private:
    static std::vector<std::string> keyboardInputText;
    static std::vector<std::string> mouseInputText;
	static m_Keys key;
};