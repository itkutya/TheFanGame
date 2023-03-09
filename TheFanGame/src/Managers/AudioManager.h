#pragma once

#include <array>

#include "ResourceManager.h"

//TODO:
//Load music title froms settings...
class AudioManager
{
public:
    AudioManager(AudioManager const&) = delete;
    void operator=(AudioManager const&) = delete;
    virtual ~AudioManager() noexcept = default;

    [[nodiscard]] static AudioManager& getInstance();

    [[nodiscard]] bool replaceCurrentMusic(const std::string& title) noexcept;
    [[nodiscard]] bool addSoundEffect(const std::string& title) noexcept;

    std::string m_CurrentMusicTitle = "Blackbird - Cecile Corbel";
    const std::array<const std::string, 2> m_MusicTitles = { "Blackbird - Cecile Corbel", "Sakakibara Yui - Koi no Honoo" };
    std::shared_ptr<sf::Music> m_CurrentMusic;
    std::unordered_map<std::string, std::shared_ptr<ResourceManager::AudioObject>> m_SoundEffects;
    //TODO:
    //std::unordered_map<std::string, std::shared_ptr<ResourceManager::AudioObject>> m_3DSoundEffects;
private:
    explicit AudioManager() noexcept = default;

    ResourceManager* s_ResourceManager = &ResourceManager::getInstance();
};