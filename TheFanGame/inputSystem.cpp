#include "inputSystem.h"

inputSystem::inputSystem() noexcept
{
    for (std::uint32_t i = 0; i < sf::Joystick::Count; ++i)
        this->m_isAnyJoystickConnected |= sf::Joystick::isConnected(i);
}

const bool inputSystem::checkForInput(const std::string& action) const noexcept
{
    if (sf::Keyboard::isKeyPressed(inputSystem::m_Action[action].m_KeyCode))
        return true;
    if (sf::Mouse::isButtonPressed(inputSystem::m_Action[action].m_MouseButton))
        return true;
    if (this->m_isAnyJoystickConnected)
    {
        for (std::uint32_t i = 0; i < sf::Joystick::Count; ++i)
        {
            if (sf::Joystick::isButtonPressed(i, inputSystem::m_Action[action].m_joystickButton) || 
                sf::Joystick::getAxisPosition(i, inputSystem::m_Action[action].m_JoystickAxis))
                return true;
        }
    }
    return false;
}

const bool inputSystem::checkForInput(const std::string& action, sf::Event& event) const noexcept
{
    if (inputSystem::m_Action[action].m_InputType == InputType::MouseInput &&
        inputSystem::m_Action[action].m_EventType == event.type &&
        inputSystem::m_Action[action].m_MouseButton == event.mouseButton.button)
        return true;
    if (inputSystem::m_Action[action].m_InputType == InputType::KeyboardInput &&
        inputSystem::m_Action[action].m_EventType == event.type &&
        inputSystem::m_Action[action].m_KeyCode == event.key.code)
        return true;
    if (inputSystem::m_Action[action].m_InputType == InputType::JoystickButtonInput &&
        inputSystem::m_Action[action].m_EventType == event.type &&
        inputSystem::m_Action[action].m_KeyCode == event.key.code)
        return true;
    if (inputSystem::m_Action[action].m_InputType == InputType::JoystickAxisInput &&
        inputSystem::m_Action[action].m_EventType == event.type &&
        inputSystem::m_Action[action].m_KeyCode == event.key.code)
        return true;
    return false;
}

const bool inputSystem::loadInput(const std::string& filePath) noexcept
{
    Input key = Input();
    std::ifstream inputSettings(filePath);
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
                {
                    inputSettings.close();
                    return false;
                }

                key.m_KeyCode = static_cast<sf::Keyboard::Key>(temp);
                inputSystem::m_Action[action] = key;
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
                {
                    inputSettings.close();
                    return false;
                }

                key.m_MouseButton = static_cast<sf::Mouse::Button>(temp);
                inputSystem::m_Action[action] = key;
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
                {
                    inputSettings.close();
                    return false;
                }

                key.m_joystickButton = temp;
                inputSystem::m_Action[action] = key;
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
                {
                    inputSettings.close();
                    return false;
                }

                key.m_JoystickAxis = static_cast<sf::Joystick::Axis>(temp);
                inputSystem::m_Action[action] = key;
                break;
            }
            default:
                break;
            }
        }
        inputSettings.close();
        return true;
    }
    inputSettings.close();
    return false;
}

const bool inputSystem::saveInput(const std::pair<std::string, Input>& temp)const noexcept
{
    inputSystem::m_Action[temp.first] = temp.second;
    std::ofstream inputSettings("res/inputSettings.ini", std::ios::trunc);
    if (inputSettings.is_open())
    {
        for (auto& it : m_Action)
        {
            switch (it.second.m_InputType)
            {
            case InputType::KeyboardInput:
                switch (it.second.m_EventType)
                {
                case sf::Event::KeyPressed:
                    inputSettings << 0 << " " << it.first << " " << it.second.m_KeyCode << " P" << '\n';
                    break;
                case sf::Event::KeyReleased:
                    inputSettings << 0 << " " << it.first << " " << it.second.m_KeyCode << " R" << '\n';
                    break;
                [[unlikely]] default: break;
                }
                break;
            case InputType::MouseInput:
                switch (it.second.m_EventType)
                {
                case sf::Event::MouseButtonPressed:
                    inputSettings << 1 << " " << it.first << " " << it.second.m_MouseButton << " P" << '\n';
                    break;
                case sf::Event::MouseButtonReleased:
                    inputSettings << 1 << " " << it.first << " " << it.second.m_MouseButton << " R" << '\n';
                    break;
                case sf::Event::MouseMoved:
                    inputSettings << 1 << " " << it.first << " " << it.second.m_MouseButton << " M" << '\n';
                    break;
                [[unlikely]] default: break;
                }
                break;
            case InputType::JoystickButtonInput:
                switch (it.second.m_EventType)
                {
                case sf::Event::JoystickButtonPressed:
                    inputSettings << 2 << " " << it.first << " " << it.second.m_joystickButton << " P" << '\n';
                    break;
                case sf::Event::JoystickButtonReleased:
                    inputSettings << 2 << " " << it.first << " " << it.second.m_joystickButton << " R" << '\n';
                    break;
                case sf::Event::JoystickMoved:
                    inputSettings << 2 << " " << it.first << " " << it.second.m_joystickButton << " M" << '\n';
                    break;
                [[unlikely]] default: break;
                }
                break;
            case InputType::JoystickAxisInput:
                switch (it.second.m_EventType)
                {
                case sf::Event::JoystickButtonPressed:
                    inputSettings << 3 << " " << it.first << " " << it.second.m_JoystickAxis << " P" << '\n';
                    break;
                case sf::Event::JoystickButtonReleased:
                    inputSettings << 3 << " " << it.first << " " << it.second.m_JoystickAxis << " R" << '\n';
                    break;
                case sf::Event::JoystickMoved:
                    inputSettings << 3 << " " << it.first << " " << it.second.m_JoystickAxis << " M" << '\n';
                    break;
                [[unlikely]] default: break;
                }
                break;
            [[unlikely]] default: break;
            }
        }
        inputSettings.close();
        return true;
    }
    inputSettings.close();
    return false;
}

const std::string inputSystem::keyToString(const Input& it) const noexcept
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
        [[unlikely]] default: break;
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
        [[unlikely]] default: break;
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

const std::string inputSystem::eventToString(const Input& it) const noexcept
{
    if (it.m_EventType == sf::Event::KeyPressed || it.m_EventType == sf::Event::MouseButtonPressed || it.m_EventType == sf::Event::JoystickButtonPressed)
        return "Press";
    else if (it.m_EventType == sf::Event::KeyReleased || it.m_EventType == sf::Event::MouseButtonReleased || it.m_EventType == sf::Event::JoystickButtonReleased)
        return "Release";
    else if (it.m_EventType == sf::Event::JoystickMoved)
        return "Move";
    return "ERROR!";
}

const std::optional<Input> inputSystem::checkForAnyKeyboardInput() const noexcept
{

    for (std::size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
    {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i)))
        {
            Input key = Input();
            key.m_InputType = InputType::KeyboardInput;
            key.m_KeyCode = static_cast<sf::Keyboard::Key>(i);
            key.m_EventType = sf::Event::KeyPressed;
            return key;
        }
    }
    return std::nullopt;
}

const std::optional<Input> inputSystem::checkForAnyMouseInput() const noexcept
{
    for (std::size_t i = 0; i < sf::Mouse::ButtonCount; ++i)
    {
        if (sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i)))
        {
            Input key = Input();
            key.m_InputType = InputType::MouseInput;
            key.m_MouseButton = static_cast<sf::Mouse::Button>(i);
            key.m_EventType = sf::Event::MouseButtonPressed;
            return key;
        }
    }
    return std::nullopt;
}

const std::optional<Input> inputSystem::checkForAnyJoystickInput() const noexcept
{
    for (std::uint32_t i = 0; i < sf::Joystick::Count; ++i)
    {
        if (sf::Joystick::isConnected(i))
        {
            for (std::uint32_t j = 0; j < sf::Joystick::ButtonCount; ++j)
            {
                if (sf::Joystick::isButtonPressed(i, j))
                {
                    Input key = Input();
                    key.m_InputType = InputType::JoystickButtonInput;
                    key.m_joystickButton = j;
                    key.m_EventType = sf::Event::JoystickButtonPressed;
                    return key;
                }
            }
            for (std::size_t j = 0; j < sf::Joystick::AxisCount; ++j)
            {
                sf::Joystick::Axis tempAxis = static_cast<sf::Joystick::Axis>(j);
                if (sf::Joystick::getAxisPosition(i, tempAxis) > 0.f)
                {
                    Input key = Input();
                    key.m_InputType = InputType::JoystickAxisInput;
                    key.m_JoystickAxis = tempAxis;
                    key.m_EventType = sf::Event::JoystickMoved;
                    return key;
                }
            }
        }
    }
    return std::nullopt;
}

const bool inputSystem::checkForAnyInput() const noexcept
{
    this->checkForAnyKeyboardInput();
    this->checkForAnyMouseInput();
    this->checkForAnyJoystickInput();
    if (this->checkForAnyKeyboardInput().has_value() ||
        this->checkForAnyMouseInput().has_value() ||
        this->checkForAnyJoystickInput().has_value())
        return true;
    return false;
}