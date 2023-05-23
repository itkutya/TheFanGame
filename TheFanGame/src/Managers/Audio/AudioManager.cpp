#include "AudioManager.h"

AudioManager::AudioManager() noexcept
{
	sf::Listener::setGlobalVolume(this->m_GlobalVolume);
}

void AudioManager::addSoundEffect(const char* title, AudioSettings as) noexcept
{
	this->m_SoundEffects.emplace(std::make_pair(title, SoundEffect(title, as)));
}