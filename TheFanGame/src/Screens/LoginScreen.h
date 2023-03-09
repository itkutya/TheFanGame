#pragma once

#include "RegisterScreen.h"
#include "MainScreen.h"
#include "Settings.h"

class LoginScreen : public State
{
public:
    explicit LoginScreen() noexcept = default;
    virtual ~LoginScreen() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;

    bool LoginAccount() noexcept;
private:
    Account* s_Account = &Account::getInstance();
    StateManager* s_StateManager = &StateManager::getInstance();
    FileManager* s_FileManager = &FileManager::getInstance();

    //TODO:
    //Maybie do this with the rest of them...
    Settings& s_Settings = Settings::getInstance();
};