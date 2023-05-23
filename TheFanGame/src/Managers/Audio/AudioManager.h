#pragma once

#include "Music.h"
#include "Sound.h"

class AudioManager : public Singleton<AudioManager>
{
    friend class Singleton<AudioManager>;

    ResourceManager& s_ResourceManager = ResourceManager::getInstance();
    SettingsManager& s_SettingsManager = SettingsManager::getInstance();
protected:
    AudioManager() noexcept;
    ~AudioManager() noexcept;
public:
    void addSoundEffect(const char* title, AudioSettings as = { false }) noexcept;

    float& m_GlobalVolume = this->s_SettingsManager["Audio"]["GlobalVolume"];
    BackGroundMusic m_BackGroundMusic;
    std::unordered_map<std::string_view, SoundEffect> m_SoundEffects;
    float& m_SoundEffectVolume = this->s_SettingsManager["Audio"]["SoundEffectVolume"];
private:
};