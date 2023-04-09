#pragma once

#include "Account/Account.h"
#include "Managers/StateManager.h"

class ShopScreen : public State, public PopUpState
{
    Account& s_Account = Account::getInstance();
    enum class SHOP_STATE
    {
        SALES, CHARACTERS, ICONS, BACKGROUND, OTHERSTUFF
    }; SHOP_STATE m_state = SHOP_STATE::SALES;
public:
    ShopScreen(Application* app) noexcept {};

    virtual void init(sf::RenderWindow& window) override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
};