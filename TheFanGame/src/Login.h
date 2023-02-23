#pragma once

#include "Register.h"

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
    Account* s_Account;
    StateManager* s_StateManager;
};