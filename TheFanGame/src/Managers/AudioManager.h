#pragma once

#include <array>

#include "SettingsManager.h"
#include "ResourceManager.h"

class AudioManager
{
    ResourceManager& s_ResourceManager = ResourceManager::getInstance();
    SettingsManager& s_Settings = SettingsManager::getInstance();
public:
    AudioManager(AudioManager const&) = delete;
    void operator=(AudioManager const&) = delete;
    virtual ~AudioManager() noexcept = default;

    [[nodiscard]] static AudioManager& getInstance();

    [[nodiscard]] bool replaceCurrentMusic(const std::string& title) noexcept;
    [[nodiscard]] bool addSoundEffect(const std::string& title) noexcept;

    const std::array<const std::string, 2> m_MusicTitles = { "Blackbird - Cecile Corbel", "Sakakibara Yui - Koi no Honoo" };
    std::string& m_CurrentMusicTitle = this->s_Settings["Audio"]["MusicTitle"];
    std::shared_ptr<sf::Music> m_CurrentMusic;
    std::unordered_map<std::string, std::shared_ptr<ResourceManager::AudioObject>> m_SoundEffects;
    //TODO:
    //std::unordered_map<std::string, std::shared_ptr<ResourceManager::AudioObject>> m_3DSoundEffects;

    float m_musicvolume = 20.f;
    float m_sfxvolume = 20.f;
    float m_gamevolume = 20.f;
private:
    explicit AudioManager() noexcept = default;
};