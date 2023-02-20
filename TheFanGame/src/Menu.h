#pragma once

#include "ResourceManager.h"
#include "StateManager.h"

#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_notify.h"
#include "tahoma.h"
#include "imgui_stdlib.h"

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
};

