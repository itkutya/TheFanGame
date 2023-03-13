#include "AudioManager.h"

AudioManager& AudioManager::getInstance()
{
	static AudioManager instance;
	return instance;
}

bool AudioManager::replaceCurrentMusic(const std::string& title) noexcept
{
	if (this->m_CurrentMusic)
		this->m_CurrentMusic->stop();
	this->m_CurrentMusic = this->s_ResourceManager.add<sf::Music>(title);
	if (this->m_CurrentMusic->openFromFile("Resources/Musics/" + title + ".wav"))
	{
		this->m_CurrentMusicTitle = title;
		this->m_CurrentMusic->setLoop(true);
		this->m_CurrentMusic->setVolume(this->m_musicvolume);
		this->m_CurrentMusic->play();
		return true;
	}
	this->m_CurrentMusic.reset();
	if (!this->s_ResourceManager.remove<sf::Music>(title))
		std::printf("Failed to deallocate Music memory!");
	return false;
}

bool AudioManager::addSoundEffect(const std::string& title) noexcept
{
	this->m_SoundEffects[title] = this->s_ResourceManager.add<ResourceManager::AudioObject>(title);
	if (this->m_SoundEffects[title]->Buffer.loadFromFile("Resources/SoundEffects/" + title + ".wav"))
	{
		this->m_SoundEffects[title]->Sound.setBuffer(this->m_SoundEffects[title]->Buffer);
		this->m_SoundEffects[title]->Sound.setVolume(this->m_sfxvolume);
		return true;
	}
	this->m_SoundEffects[title].reset();
	this->m_SoundEffects.erase(title);
	if (!this->s_ResourceManager.remove<ResourceManager::AudioObject>(title))
		std::printf("Failed to deallocate AudioObject memory!");
	return false;
}