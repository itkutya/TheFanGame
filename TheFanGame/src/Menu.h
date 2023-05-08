#pragma once

#include "Screens/LoginScreen.h"

class Menu : public State
{
    ResourceManager& s_ResourceManager = ResourceManager::getInstance();
    StateManager& s_StateManager = StateManager::getInstance();
public:
    Menu(Application* app) noexcept { this->m_app = app; };

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;
private:
    ResourceManager::Object* m_BackgroundImage = nullptr;
};