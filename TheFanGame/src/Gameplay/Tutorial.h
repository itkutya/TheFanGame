#pragma once

#include "Game.h"

class Tutorial : public State
{
public:
    Tutorial(Application* app) noexcept { this->m_app = app; };

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;
private:
    Dungeon m_dungeon;
};