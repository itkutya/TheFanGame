#include "inputSystem.h"

std::unordered_map<std::string, m_Keys> inputSystem::m_Action;
bool inputSystem::isJoystickConnected = false;

const void inputSystem::init()
{
    m_Keys key = m_Keys();

    std::ifstream inputSettings("res/inputSettings.ini");
    if (inputSettings.is_open())
    {
        std::uint32_t type;
        std::string action;
        std::uint32_t temp;
        std::string pr;

        while (inputSettings >> type >> action >> temp >> pr)
        {
            InputType tempType = static_cast<InputType>(type);

            switch (tempType)
            {
            case InputType::KeyboardInput:
            {
                key.m_InputType = InputType::KeyboardInput;

                if (pr == "P")
                    key.m_EventType = sf::Event::KeyPressed;
                else if (pr == "R")
                    key.m_EventType = sf::Event::KeyReleased;
                else
                    std::cout << "Error! Unknown input type.\n";

                key.m_KeyCode = static_cast<sf::Keyboard::Key>(temp);
                m_Action[action] = key;
                break;
            }
            case InputType::MouseInput:
            {
                key.m_InputType = InputType::MouseInput;

                if (pr == "P")
                    key.m_EventType = sf::Event::MouseButtonPressed;
                else if (pr == "R")
                    key.m_EventType = sf::Event::MouseButtonReleased;
                else
                    std::cout << "Error! Unknown input type.\n";

                key.m_MouseButton = static_cast<sf::Mouse::Button>(temp);
                m_Action[action] = key;
                break;
            }
            case InputType::JoystickButtonInput:
            {
                key.m_InputType = InputType::JoystickButtonInput;

                if (pr == "P")
                    key.m_EventType = sf::Event::JoystickButtonPressed;
                else if (pr == "R")
                    key.m_EventType = sf::Event::JoystickButtonReleased;
                else if (pr == "M")
                    key.m_EventType = sf::Event::JoystickMoved;
                else
                    throw "Error while loading from the input settings file...\n";

                key.m_joystickButton = temp;
                m_Action[action] = key;
                break;
            }
            case InputType::JoystickAxisInput:
            {

                key.m_InputType = InputType::JoystickAxisInput;

                if (pr == "P")
                    key.m_EventType = sf::Event::JoystickButtonPressed;
                else if (pr == "R")
                    key.m_EventType = sf::Event::JoystickButtonReleased;
                else if (pr == "M")
                    key.m_EventType = sf::Event::JoystickMoved;
                else
                    throw "Error while loading from the input settings file...\n";

                key.m_JoystickAxis = static_cast<sf::Joystick::Axis>(temp);
                m_Action[action] = key;
                break;
            }
            default:
                break;
            }
        }
    }
    else
        throw "Cannot load input settings...\n";
    inputSettings.close();
}

const bool inputSystem::input(m_Keys& key, sf::Event* event) noexcept
{
    if (event == nullptr)
    {
        if (sf::Keyboard::isKeyPressed(key.m_KeyCode))
            return true;
        if (sf::Mouse::isButtonPressed(key.m_MouseButton))
            return true;
        if (isJoystickConnected)
        {
            for (std::uint32_t i = 0; i < sf::Joystick::Count; ++i)
            {
                if (sf::Joystick::isButtonPressed(i, key.m_joystickButton))
                    return true;
            }
        }
    }
    else
    {
        if (key.m_InputType == InputType::MouseInput &&
            key.m_EventType == event->type &&
            key.m_MouseButton == event->mouseButton.button)
            return true;
        if (key.m_InputType == InputType::KeyboardInput &&
            key.m_EventType == event->type &&
            key.m_KeyCode == event->key.code)
            return true;
        if (key.m_InputType == InputType::JoystickButtonInput &&
            key.m_EventType == event->type &&
            key.m_KeyCode == event->key.code)
            return true;
    }
    return false;
}

const char* inputSystem::convert(const m_Keys& it)
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
        case sf::Keyboard::F12:
            return "F12";
        default:
            break;
        }
    }
    else if(it.m_InputType == InputType::MouseInput)
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
}

const char* inputSystem::PressOrRelease(const m_Keys& it)
{
    if (it.m_EventType == sf::Event::KeyPressed || it.m_EventType == sf::Event::MouseButtonPressed || it.m_EventType == sf::Event::JoystickButtonPressed)
        return "Press";
    else if (it.m_EventType == sf::Event::KeyReleased || it.m_EventType == sf::Event::MouseButtonReleased || it.m_EventType == sf::Event::JoystickButtonReleased)
        return "Release";
    else if (it.m_EventType == sf::Event::JoystickMoved)
        return "Move";
    return "ERROR!";
}

const void inputSystem::saveInput(const std::pair<std::string, m_Keys>& temp) noexcept
{
    m_Action.at(temp.first) = temp.second;
}

const void inputSystem::clear() noexcept { m_Action.clear(); }
