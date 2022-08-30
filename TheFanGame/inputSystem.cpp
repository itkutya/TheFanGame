#include "inputSystem.h"

std::unordered_map<const char*, m_Keys> inputSystem::m_Action;
bool inputSystem::isJoystickConnected = false;

const void inputSystem::init()
{
    constexpr std::array keyboardInputText = { "Left", "Right", "Forward", "Backward", "ScreenShot" };
    constexpr std::array mouseInputText = { "Fire", "Scoop" };

    m_Keys key = m_Keys();

    std::ifstream inputSettings;
    inputSettings.open("res/inputSettings.ini", std::ios::in);
    if (inputSettings.is_open())
    {
        std::uint32_t temp;
        std::string pr;

        key.m_InputType = InputType::KeyboardInput;
        for (std::size_t i = 0; i < keyboardInputText.size(); ++i)
        {
            inputSettings >> temp >> pr;

            if (pr == "P")
                key.m_EventType = sf::Event::KeyPressed;
            else if (pr == "R")
                key.m_EventType = sf::Event::KeyReleased;
            else
                throw "Error while loading from the input settings file...\n";

            sf::Keyboard::Key m_TempKey = sf::Keyboard::Unknown;
            m_TempKey = static_cast<sf::Keyboard::Key>(temp);
            key.m_KeyCode = m_TempKey;
            m_Action[keyboardInputText[i]] = key;
        }

        key.m_InputType = InputType::MouseInput;
        key.m_KeyCode = sf::Keyboard::Unknown;
        for (std::size_t i = 0; i < mouseInputText.size(); ++i)
        {
            inputSettings >> temp >> pr;

            if (pr == "P")
                key.m_EventType = sf::Event::MouseButtonPressed;
            else if (pr == "R")
                key.m_EventType = sf::Event::MouseButtonReleased;
            else
                throw "Error while loading from the input settings file...\n";

            sf::Mouse::Button m_TempMouseButton = sf::Mouse::ButtonCount;
            m_TempMouseButton = static_cast<sf::Mouse::Button>(temp);
            key.m_MouseButton = m_TempMouseButton;
            //The warning will go away if you assigne more than 1 const char* to the array....
            m_Action[mouseInputText[i]] = key;
        }

        key.m_InputType = InputType::JoystickInput;
        key.m_MouseButton = sf::Mouse::ButtonCount;
        bool isAxis = false;
        for (std::uint32_t i = 0; i < sf::Joystick::Count; ++i)
        {
            if (sf::Joystick::isConnected(i))
            {
                isJoystickConnected = true;
                //Write default joystick data to the file so it wont crash / throw...
                constexpr std::array joystickInputText = { "Left", "Right", "Forward", "Backward", "ScreenShot", "Fire" };
                for (std::size_t i = 0; i < joystickInputText.size(); ++i)
                {
                    inputSettings >> isAxis >> temp >> pr;

                    if (pr == "P")
                        key.m_EventType = sf::Event::JoystickButtonPressed;
                    else if (pr == "R")
                        key.m_EventType = sf::Event::JoystickButtonReleased;
                    else if (pr == "M")
                        key.m_EventType = sf::Event::JoystickMoved;
                    else
                        throw "Error while loading from the input settings file...\n";

                    if (isAxis)
                    {
                        sf::Joystick::Axis m_TempJoystickAxis = sf::Joystick::Axis::V;
                        m_TempJoystickAxis = static_cast<sf::Joystick::Axis>(temp);
                        key.m_JoystickAxis = m_TempJoystickAxis;
                        m_Action[joystickInputText[i]] = key;
                    }
                    else
                    {
                        key.m_joystickButton = temp;
                        m_Action[joystickInputText[i]] = key;
                    }
                }
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
        if (key.m_InputType == InputType::JoystickInput &&
            key.m_EventType == event->type &&
            key.m_KeyCode == event->key.code)
            return true;
    }
    return false;
}

const char* inputSystem::convert(const m_Keys& it)
{
    if (it.m_KeyCode != sf::Keyboard::Unknown)
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
    else
    {
        switch (it.m_MouseButton)
        {
        case sf::Mouse::Left:
            return "Mouse Left";
        default:
            break;
        }
    }
    return "ERROR";
}

const void inputSystem::clear() noexcept { m_Action.clear(); }