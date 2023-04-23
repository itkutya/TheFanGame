#pragma once

#include <vector>

#include "Screens/LoginScreen.h"
#include "Managers/Input/InputManager.h"

class Menu : public State
{
    StateManager& s_StateManager = StateManager::getInstance();
    ResourceManager& s_ResourceManager = ResourceManager::getInstance();
    InputManager& s_InputManager = InputManager::getInstance();
public:
    Menu(Application* app) noexcept : m_app(app) {};

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;
private:
    Application* m_app;

    ResourceManager::Object* m_BackgroundImage = nullptr;
};