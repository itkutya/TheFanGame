#pragma once

#include "Account/Account.h"
#include "Managers/StateManager.h"
#include "Managers/CharactersManager.h"

class ShopScreen : public PopupGUIState
{
    Account& s_Account = Account::getInstance();
    CharactersManager& s_CharactersManager = CharactersManager::getInstance();
    enum class SHOP_STATE
    {
        SALES, CHARACTERS, ICONS, BACKGROUND, OTHERSTUFF
    }; SHOP_STATE m_state = SHOP_STATE::SALES;
public:
    ShopScreen(Application* app) noexcept { this->m_app = app; this->m_name = "Shop"; };

    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
private:
};