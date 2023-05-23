#pragma once

#include "Managers/Setting/SettingsManager.h"
#include "Managers/ResourceManager.h"

class SoundEffect
{
protected:
	ResourceManager& s_ResourceManager = ResourceManager::getInstance();

	SoundEffect() noexcept = default;

	[[nodiscard]] virtual const bool change(const char* title, AudioSettings ms) noexcept;
public:
	SoundEffect(const char* title, AudioSettings ms = { false }) noexcept;
	virtual ~SoundEffect() noexcept = default;

	virtual void Play() noexcept;
	virtual void Stop() noexcept;

	ResourceManager::AudioObject* m_SoundEffect = nullptr;
private:
	AudioSettings m_AudioSettings;
};

class SoundEffect3D : public SoundEffect
{
public:
	SoundEffect3D(const char* title, sf::Vector3f pos, AudioSettings ms = { false }) noexcept;
	~SoundEffect3D() noexcept = default;
private:
	sf::Vector3f m_position;
};