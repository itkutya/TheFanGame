#pragma once

#include "Managers/StateManager.h"

#include "World/Dungeon.h"

class Game : public State
{
public:
    Game(Application* app) noexcept { this->m_app = app; };

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;
private:
};