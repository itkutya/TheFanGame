#pragma once

#include "Managers/ResourceManager.h"
#include "Managers/SettingsManager.h"
#include "Managers/StateManager.h"
#include "Managers/AudioManager.h"
#include "Account/Account.h"

class SettingsScreen : public State, public PopUpState
{
public:
    explicit SettingsScreen() noexcept = default;
    virtual ~SettingsScreen() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
private:
    SettingsManager& s_Settings = SettingsManager::getInstance();

    std::shared_ptr<ResourceManager::Object> m_Icon;
    std::shared_ptr<ResourceManager::Object> m_FrontImage;
    std::shared_ptr<sf::Music> m_Music;
};