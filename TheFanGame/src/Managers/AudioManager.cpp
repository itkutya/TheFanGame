#include "AudioManager.h"

AudioManager& AudioManager::getInstance()
{
	static AudioManager instance;
	return instance;
}

bool AudioManager::replaceCurrentMusic(const std::string_view title) noexcept
{
	if (this->m_CurrentMusic)
		this->m_CurrentMusic->stop();
	this->m_CurrentMusic = this->s_ResourceManager.add<sf::Music>(title);
	const auto& path = std::format("Resources/Musics/{}.wav", title.data());
	if (this->m_CurrentMusic->openFromFile(path.c_str()))
	{
		this->m_CurrentMusicTitle = title;
		this->m_CurrentMusic->setLoop(true);
		this->m_CurrentMusic->setVolume(this->m_musicvolume);
		this->m_CurrentMusic->play();
		return true;
	}
	this->m_CurrentMusic = nullptr;
	this->s_ResourceManager.remove<sf::Music>(title);
	return false;
}

bool AudioManager::addSoundEffect(const std::string_view title) noexcept
{
	this->m_SoundEffects[title] = this->s_ResourceManager.add<ResourceManager::AudioObject>(title);
	const auto& path = std::format("Resources/SoundEffects/{}.wav", title.data());
	if (this->m_SoundEffects[title]->Buffer.loadFromFile(path.c_str()))
	{
		this->m_SoundEffects[title]->Sound.setBuffer(this->m_SoundEffects[title]->Buffer);
		this->m_SoundEffects[title]->Sound.setVolume(this->m_sfxvolume);
		return true;
	}
	this->m_SoundEffects[title] = nullptr;
	this->m_SoundEffects.erase(title);
	this->s_ResourceManager.remove<ResourceManager::AudioObject>(title);
	return false;
}