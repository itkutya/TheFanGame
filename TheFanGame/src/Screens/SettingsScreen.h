#pragma once

#include "Managers/ResourceManager.h"
#include "Managers/SettingsManager.h"
#include "Managers/StateManager.h"
#include "Managers/AudioManager.h"
#include "Account/Account.h"

#include "Application.h"

class SettingsScreen : public State, public PopUpState
{
    SettingsManager& s_Settings = SettingsManager::getInstance();
    AudioManager& s_AudioManager = AudioManager::getInstance();
    ResourceManager& s_ResourceManager = ResourceManager::getInstance();
    enum class SETTINGS_STATE
    {
        GRAPHICS, PROFILE, AUDIO, INPUT, GAME, MAINSCREEN
    };SETTINGS_STATE m_state = SETTINGS_STATE::MAINSCREEN;
public:
    explicit SettingsScreen(Application* app) noexcept : m_app(app) {};
    virtual ~SettingsScreen() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
private:
    std::shared_ptr<ResourceManager::Object> m_Icon = s_ResourceManager.get<ResourceManager::Object>("Icon");
    std::shared_ptr<ResourceManager::Object> m_FrontImage = s_ResourceManager.get<ResourceManager::Object>("FrontImage");
    std::shared_ptr<ResourceManager::Object> m_BackgroundImage = s_ResourceManager.get<ResourceManager::Object>("BackgroundImage");
    std::shared_ptr<sf::Music> m_Music;

    Application* m_app;

    int m_currFrontImage = 0;
    int m_currBackgroundImage = 0;
    int m_currIcon = 0;
};