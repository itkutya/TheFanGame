#pragma once

#include <array>

#include "Managers/Setting/SettingsManager.h"
#include "Managers/ResourceManager.h"

class Music
{
protected:
	ResourceManager& s_ResourceManager = ResourceManager::getInstance();

	Music() = default;
public:
	Music(const char* title, AudioSettings ms = { false }) noexcept;
	virtual ~Music() noexcept = default;

	sf::Music* m_Music = nullptr;
private:
	std::string m_MusicTitle;
	AudioSettings m_AudioSettings;
};

class BackGroundMusic : public Music
{
	SettingsManager& s_SettingsManager = SettingsManager::getInstance();
public:
	BackGroundMusic() noexcept;
	~BackGroundMusic() noexcept = default;

	void update() noexcept;
	[[nodiscard]] const bool change(const char* title) noexcept;

	const std::array<const char*, 2> m_MusicTitles = { "Blackbird_-_Cecile_Corbel", "Sakakibara_Yui_-_Koi_no_Honoo" };
	std::string& m_CurrentTitle = this->s_SettingsManager["Audio"]["MusicTitle"];
	bool& m_random = this->s_SettingsManager["Audio"]["RandomBackGroundMusic"];
	float& m_volume = this->s_SettingsManager["Audio"]["BackGroundMusicVolume"];
};

class Music3D : public Music
{
public:
	Music3D(sf::Vector3f pos) noexcept;
	~Music3D() noexcept = default;
private:
	sf::Vector3f m_position;
};