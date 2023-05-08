#pragma once

#include "RegisterScreen.h"
#include "MainScreen.h"

class LoginScreen : public GUIState
{
    StateManager& s_StateManager = StateManager::getInstance();
    Account& s_Account = Account::getInstance();
public:
    LoginScreen(Application* app) noexcept { this->m_app = app; };

    virtual void init(sf::RenderWindow& window) override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
private:
};