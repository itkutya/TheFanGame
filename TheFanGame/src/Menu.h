#pragma once

#include <vector>

#include "Login.h"

class Menu : public State
{
public:
    explicit Menu() noexcept = default;
    virtual ~Menu() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(const sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;
private:
    StateManager* s_StateManager = &StateManager::getInstance();
};