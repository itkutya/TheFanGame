#pragma once

#include "SettingsScreen.h"

class MainScreen : public State
{
public:
    explicit MainScreen() noexcept = default;
    virtual ~MainScreen() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;
private:
    Account* s_Account = &Account::getInstance();
    ResourceManager* s_ResourceManager = &ResourceManager::getInstance();
    StateManager* s_StateManager = &StateManager::getInstance();
    AudioManager* s_AudioManager = &AudioManager::getInstance();

    bool m_PlaySelected = false;
    std::shared_ptr<ResourceManager::Object> m_Icon;
    std::shared_ptr<ResourceManager::Object> m_FrontImage;
    std::shared_ptr<sf::Texture> m_Pause;
    std::shared_ptr<sf::Texture> m_Resume;
};