#pragma once

#include "Managers/ResourceManager.h"
#include "Managers/StateManager.h"
#include "Account/Account.h"

class RegisterScreen : public State, public PopUpState
{
public:
    RegisterScreen(Application* app) noexcept {};

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;

    bool RegisterAccount() noexcept;
private:
    Account& s_Account = Account::getInstance();
};