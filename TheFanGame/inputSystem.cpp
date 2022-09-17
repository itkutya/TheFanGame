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

const void inputSystem::saveInput(const std::pair<std::string, m_Keys>& temp) noexcept
{
    m_Action.at(temp.first) = temp.second;
    for (std::size_t i = 0; i < m_Action.size(); ++i)
    {

    }
}

const void inputSystem::clear() noexcept { m_Action.clear(); }