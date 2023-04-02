#pragma once

#include "Managers/StateManager.h"

class ShopScreen : public State, public PopUpState
{
public:
    ShopScreen(Application* app) noexcept {};

    virtual void init(sf::RenderWindow& window) override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
};