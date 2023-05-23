#include "AudioManager.h"

AudioManager::AudioManager() noexcept
{
	sf::Listener::setGlobalVolume(this->m_GlobalVolume);
}

AudioManager::~AudioManager() noexcept
{
	this->m_BackGroundMusic.m_Music->stop();
	for (auto& SoundEffect : this->m_SoundEffects)
		SoundEffect.second.Stop();
}

void AudioManager::addSoundEffect(const char* title, AudioSettings as) noexcept
{
	this->m_SoundEffects.emplace(std::make_pair(title, SoundEffect(title, as)));
}