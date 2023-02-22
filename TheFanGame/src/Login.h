#pragma once

#include "ResourceManager.h"
#include "StateManager.h"
#include "Account.h"

#include "imgui.h"
#include "imgui_stdlib.h"

class Login : public State
{
public:
    explicit Login() noexcept = default;
    virtual ~Login() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(const sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;
private:
    Account* s_Account = &Account::getInstance();
};

