#pragma once

#include "Managers/SettingsManager.h"
#include "RegisterScreen.h"
#include "MainScreen.h"

class LoginScreen : public State
{
public:
    LoginScreen(Application* app) noexcept : m_app(app) {};

    virtual void init(sf::RenderWindow& window) override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
private:
    Account& s_Account = Account::getInstance();
    StateManager& s_StateManager = StateManager::getInstance();

    Application* m_app;
};