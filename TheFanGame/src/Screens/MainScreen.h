#pragma once

#include "SettingsScreen.h"
#include "ShopScreen.h"

class MainScreen : public State
{
    Account& s_Account = Account::getInstance();
    ResourceManager& s_ResourceManager = ResourceManager::getInstance();
    StateManager& s_StateManager = StateManager::getInstance();
    AudioManager& s_AudioManager = AudioManager::getInstance();
public:
    MainScreen(Application* app) noexcept : m_app(app) {};

    virtual void init(sf::RenderWindow& window) override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
private:
    bool m_PlaySelected = false;
    ResourceManager::Object* m_Icon;
    ResourceManager::Object* m_FrontImage;
    sf::Texture* m_Pause;
    sf::Texture* m_Resume;

    Application* m_app;
};