#pragma once

#include <vector>

#include "Screens/LoginScreen.h"

class Menu : public State
{
public:
    explicit Menu(Application* app) noexcept : m_app(app) {};
    virtual ~Menu() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;
private:
    StateManager& s_StateManager = StateManager::getInstance();

    Application* m_app;
};