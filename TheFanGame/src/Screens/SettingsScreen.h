#pragma once

#include "Managers/ResourceManager.h"
#include "Managers/StateManager.h"
#include "Managers/AudioManager.h"
#include "Managers/FileManager.h"
#include "Account/Account.h"

class SettingsScreen : public State, public PopUpState
{
public:
    explicit SettingsScreen() noexcept = default;
    virtual ~SettingsScreen() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;
private:
    Account* s_Account = &Account::getInstance();
    ResourceManager* s_ResourceManager = &ResourceManager::getInstance();
    FileManager* s_FileManager = &FileManager::getInstance();

    std::shared_ptr<ResourceManager::Object> m_Icon;
    std::shared_ptr<ResourceManager::Object> m_FrontImage;
    std::shared_ptr<sf::Music> m_Music;
};