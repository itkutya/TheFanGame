#include "Music.h"

#include <format>
#include <random>

Music::Music(const char* title, AudioSettings ms) noexcept : m_MusicTitle(title), m_AudioSettings(ms)
{
	std::unique_ptr<sf::Music> temp = std::make_unique<sf::Music>();
	if (temp->openFromFile(std::format("Resources/Musics/{}.wav", title).c_str()))
	{
		this->s_ResourceManager.add<sf::Music>(title);
		this->s_ResourceManager.getUniquePtr<sf::Music>(title) = std::move(temp);

		this->m_Music = this->s_ResourceManager.get<sf::Music>(title);
		this->m_Music->setLoop(ms.m_loop);
		this->m_Music->setVolume(ms.m_volume);
		this->m_Music->setPosition({ 0.f, 0.f, 0.f });
		this->m_Music->setRelativeToListener(true);
	}
}

BackGroundMusic::BackGroundMusic() noexcept
{
	if (!this->change(this->m_CurrentTitle.c_str()))
		std::printf("Error while loading the bgm!\n");
}

const bool BackGroundMusic::change(const char* title) noexcept
{
	std::unique_ptr<sf::Music> temp = std::make_unique<sf::Music>();
	const auto& path = std::format("Resources/BackGroundMusics/{}.wav", title);
	if (temp->openFromFile(path.c_str()))
	{
		if (this->m_Music)
		{
			this->m_Music->stop();
			this->m_Music = nullptr;
			this->s_ResourceManager.remove<sf::Music>(this->m_CurrentTitle.c_str());
		}
		this->s_ResourceManager.add<sf::Music>(title);
		this->s_ResourceManager.getUniquePtr<sf::Music>(title) = std::move(temp);

		this->m_CurrentTitle = title;
		this->m_Music = this->s_ResourceManager.get<sf::Music>(title);
		this->m_Music->setLoop(this->m_random);
		this->m_Music->setVolume(this->m_volume);
		this->m_Music->setPosition({ 0.f, 0.f, 0.f });
		this->m_Music->setRelativeToListener(true);
		if (this->m_volume)
			this->m_Music->play();

		return true;
	}
	return false;
}

void BackGroundMusic::update() noexcept
{
	if (this->m_Music && this->m_Music->getStatus() == sf::SoundSource::Status::Stopped && this->m_random && this->m_volume)
	{
		std::random_device rd;
		std::uniform_int_distribution<int>dist(0, (int)this->m_MusicTitles.size());
		if (!this->change(this->m_MusicTitles[dist(rd)]))
			std::printf("Failed to change bgm!\n");
	}
}

Music3D::Music3D(sf::Vector3f pos) noexcept : m_position(pos)
{
	this->m_Music->setRelativeToListener(false);
	this->m_Music->setPosition(this->m_position);
}