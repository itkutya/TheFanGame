#pragma once

#include <array>

#include "SettingsScreen.h"

class MainScreen : public State
{
public:
    explicit MainScreen() noexcept = default;
    virtual ~MainScreen() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(const sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;
private:
    Account* s_Account = &Account::getInstance();
    ResourceManager* s_ResourceManager = &ResourceManager::getInstance();
    StateManager* s_StateManager = &StateManager::getInstance();

    bool m_PlaySelected = false;
    std::shared_ptr<ResourceManager::Entity> m_Icon;
    std::shared_ptr<ResourceManager::Entity> m_FrontImage;
    std::shared_ptr<sf::Music> m_Music;
    std::shared_ptr<sf::Texture> m_Pause;
    std::shared_ptr<sf::Texture> m_Resume;
    std::string m_CurrentMusicTitle = "Blackbird - Cecile Corbel";
    const std::array<const std::string, 2> m_MusicTitles = { "Blackbird - Cecile Corbel", "Sakakibara Yui - Koi no Honoo" };
};