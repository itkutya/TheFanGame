#pragma once

#include <array>
#include <format>

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

    [[nodiscard]] bool replaceCurrentMusic(const std::string_view title) noexcept;
    [[nodiscard]] bool addSoundEffect(const std::string_view title) noexcept;

    const std::array<const std::string_view, 2> m_MusicTitles = { "Blackbird_-_Cecile_Corbel", "Sakakibara_Yui_-_Koi_no_Honoo" };
    std::string& m_CurrentMusicTitle = this->s_Settings["Audio"]["MusicTitle"];
    sf::Music* m_CurrentMusic = nullptr;
    std::unordered_map<std::string_view, ResourceManager::AudioObject*> m_SoundEffects;
    //TODO:
    //std::unordered_map<std::string, std::shared_ptr<ResourceManager::AudioObject>> m_3DSoundEffects;

    float m_musicvolume = 20.f;
    float m_sfxvolume = 20.f;
    float m_gamevolume = 20.f;
private:
    AudioManager() noexcept = default;
};