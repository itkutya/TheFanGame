#pragma once

#include "includes.h"

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
    inputSystem& operator=(inputSystem& other) = delete;
    inputSystem& operator=(const inputSystem& other) = delete;
	virtual ~inputSystem() = default;

    static inline const auto getInputHandler() noexcept { return m_Action; }

    static inline constexpr bool checkForInput(const std::string& action, sf::Event* event = nullptr) noexcept
    {
        if (event == nullptr)
        {
            if (sf::Keyboard::isKeyPressed(m_Action.at(action).m_KeyCode))
                return true;
            if (sf::Mouse::isButtonPressed(m_Action.at(action).m_MouseButton))
                return true;
            if (isJoystickConnected)
            {
                for (std::uint32_t i = 0; i < sf::Joystick::Count; ++i)
                {
                    if (sf::Joystick::isButtonPressed(i, m_Action.at(action).m_joystickButton) || sf::Joystick::getAxisPosition(i, m_Action.at(action).m_JoystickAxis) != 0.f)
                        return true;
                }
            }
        }
        else
        {
            if (m_Action.at(action).m_InputType == InputType::MouseInput &&
                m_Action.at(action).m_EventType == event->type &&
                m_Action.at(action).m_MouseButton == event->mouseButton.button)
                return true;
            if (m_Action.at(action).m_InputType == InputType::KeyboardInput &&
                m_Action.at(action).m_EventType == event->type &&
                m_Action.at(action).m_KeyCode == event->key.code)
                return true;
            if (m_Action.at(action).m_InputType == InputType::JoystickButtonInput &&
                m_Action.at(action).m_EventType == event->type &&
                m_Action.at(action).m_KeyCode == event->key.code)
                return true;
            if (m_Action.at(action).m_InputType == InputType::JoystickAxisInput &&
                m_Action.at(action).m_EventType == event->type &&
                m_Action.at(action).m_KeyCode == event->key.code)
                return true;
        }
        return false;
    };

    static const void loadInput(const std::string& filePath);
	static const void saveInput(const std::pair<std::string, m_Keys>& temp) noexcept;

	static inline constexpr std::string keyToString(const m_Keys& it) noexcept
	{
        if (it.m_InputType == InputType::KeyboardInput)
        {
            switch (it.m_KeyCode)
            {
            case sf::Keyboard::A:
                return "A";
            case sf::Keyboard::W:
                return "W";
            case sf::Keyboard::D:
                return "D";
            case sf::Keyboard::S:
                return "S";
            case sf::Keyboard::F8:
                return "F8";
            case sf::Keyboard::F12:
                return "F12";
            default:
                break;
            }
        }
        else if (it.m_InputType == InputType::MouseInput)
        {
            switch (it.m_MouseButton)
            {
            case sf::Mouse::Left:
                return "Mouse Left";
            case sf::Mouse::Right:
                return "Mouse Right";
            default:
                break;
            }
        }
        else if (it.m_InputType == InputType::JoystickButtonInput)
        {
            return "SoonTM";
        }
        else if (it.m_InputType == InputType::JoystickAxisInput)
        {
            return "SoonTM2";
        }
        return "ERROR";
	};

    static inline constexpr std::string eventToString(const m_Keys& it) noexcept
    {
        if (it.m_EventType == sf::Event::KeyPressed || it.m_EventType == sf::Event::MouseButtonPressed || it.m_EventType == sf::Event::JoystickButtonPressed)
            return "Press";
        else if (it.m_EventType == sf::Event::KeyReleased || it.m_EventType == sf::Event::MouseButtonReleased || it.m_EventType == sf::Event::JoystickButtonReleased)
            return "Release";
        else if (it.m_EventType == sf::Event::JoystickMoved)
            return "Move";
        return "ERROR!";
    };
	
private:
    static inline std::unordered_map<std::string, m_Keys> m_Action;
	static inline bool isJoystickConnected = false;
};