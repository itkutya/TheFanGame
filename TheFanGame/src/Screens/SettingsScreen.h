#pragma once

#include "Managers/Setting/SettingsManager.h"
#include "Managers/Input/InputManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/StateManager.h"
#include "Managers/AudioManager.h"
#include "Account/Account.h"

#include "Application.h"

class SettingsScreen : public PopupGUIState
{
    SettingsManager& s_SettingsManager = SettingsManager::getInstance();
    AudioManager& s_AudioManager = AudioManager::getInstance();
    ResourceManager& s_ResourceManager = ResourceManager::getInstance();
    StateManager& s_StateManager = StateManager::getInstance();
    InputManager& s_InputManager = InputManager::getInstance();

    enum class SETTINGS_STATE
    {
        GRAPHICS, PROFILE, AUDIO, INPUT, GAME, MAINSCREEN
    };SETTINGS_STATE m_state = SETTINGS_STATE::MAINSCREEN;
public:
    SettingsScreen(Application* app) noexcept { this->m_app = app; this->m_name = "Settings"; };

    virtual void processEvent(sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
private:
    ResourceManager::Object* m_Icon = s_ResourceManager.get<ResourceManager::Object>("Icon");
    ResourceManager::Object* m_FrontImage = s_ResourceManager.get<ResourceManager::Object>("FrontImage");
    ResourceManager::Object* m_BackgroundImage = s_ResourceManager.get<ResourceManager::Object>("BackgroundImage");
    
    int m_currFrontImage = 0;
    int m_currBackgroundImage = 0;
    int m_currIcon = 0;

    bool m_KeyBindingsPopUp = false;
    std::string m_newKey = "";
    std::unique_ptr<Input> m_newInput = std::make_unique<Input>();
};