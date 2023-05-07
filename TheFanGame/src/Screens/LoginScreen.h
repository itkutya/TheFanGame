#pragma once

#include "RegisterScreen.h"
#include "MainScreen.h"

class LoginScreen : public GUIState
{
public:
    LoginScreen(Application* app) noexcept { this->m_app = app; };

    virtual void init(sf::RenderWindow& window) override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
private:
    Account& s_Account = Account::getInstance();
};