#pragma once

#include "Managers/ResourceManager.h"
#include "Managers/StateManager.h"
#include "Account/Account.h"

class RegisterScreen : public PopupGUIState
{
public:
    RegisterScreen(Application* app) noexcept { this->m_app = app; this->m_name = "Register"; };

    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
private:
    Account& s_Account = Account::getInstance();
};