#include "inputManager.h"

std::unordered_map<std::string, m_Keys> inputManager::m_Action;
std::vector<std::string> inputManager::keyboardInputText;
std::vector<std::string> inputManager::mouseInputText;
m_Keys inputManager::key;

const void inputManager::init()
{
    keyboardInputText.push_back("Left");
    keyboardInputText.push_back("Right");
    keyboardInputText.push_back("Forward");
    keyboardInputText.push_back("Backward");
    
    mouseInputText.push_back("Shoot");

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
            m_Action[mouseInputText[i]] = key;
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