#pragma once

#include "SettingsScreen.h"
#include "ShopScreen.h"
#include "Gameplay/Tutorial.h"

class MainScreen : public GUIState
{
    ResourceManager& s_ResourceManager = ResourceManager::getInstance();
    AudioManager& s_AudioManager = AudioManager::getInstance();
    StateManager& s_StateManager = StateManager::getInstance();
    Account& s_Account = Account::getInstance();
public:
    MainScreen(Application* app) noexcept { this->m_app = app; };

    virtual void init(sf::RenderWindow& window) override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
private:
    bool m_PlaySelected = false;
    ResourceManager::Object* m_Icon = nullptr;
    ResourceManager::Object* m_FrontImage = nullptr;
};