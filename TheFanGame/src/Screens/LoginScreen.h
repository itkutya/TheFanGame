#pragma once

#include "Managers/SettingsManager.h"
#include "RegisterScreen.h"
#include "MainScreen.h"

class LoginScreen : public State
{
public:
    explicit LoginScreen(Application* app) noexcept : m_app(app) {};
    virtual ~LoginScreen() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;

    bool LoginAccount() noexcept;
private:
    Account& s_Account = Account::getInstance();
    StateManager& s_StateManager = StateManager::getInstance();
    SettingsManager& s_Settings = SettingsManager::getInstance();

    Application* m_app;
};