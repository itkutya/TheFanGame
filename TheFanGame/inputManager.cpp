#include "inputManager.h"

std::unordered_map<const char*, m_Keys> inputManager::m_Action;

const void inputManager::init()
{
    constexpr std::array keyboardInputText = { "Left", "Right", "Forward", "Backward", "ScreenShot" };
    constexpr std::array mouseInputText = { "Shot" };

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

            sf::Mouse::Button m_TempMouseButton = sf::Mouse::Left;
            m_TempMouseButton = static_cast<sf::Mouse::Button>(temp);
            key.m_MouseButton = m_TempMouseButton;

#pragma warning( push )
#pragma warning( disable : 28020)
            m_Action[mouseInputText[i]] = key;
#pragma warning( pop ) 
        }
    }
    else
        throw "Cannot load input settings...\n";
    inputSettings.close();
}

const bool inputManager::input(m_Keys& key, sf::Event* event) noexcept
{
    if (event == nullptr)
    {
        if (sf::Keyboard::isKeyPressed(key.m_KeyCode))
            return true;
        if (sf::Mouse::isButtonPressed(key.m_MouseButton))
            return true;
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
    }
    return false;
}

const char* inputManager::convert(const m_Keys& it)
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