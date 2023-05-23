#include "Sound.h"

#include <format>

SoundEffect::SoundEffect(const char* title, AudioSettings ms) noexcept
{
	if (!this->change(title, ms))
		std::printf("Failed to load %s sound effect!", title);
}

void SoundEffect::Play() noexcept
{
	this->m_SoundEffect->Sound.play();
}

void SoundEffect::Stop() noexcept
{
	this->m_SoundEffect->Sound.stop();
}

const bool SoundEffect::change(const char* title, AudioSettings ms) noexcept
{
	const auto& path = std::format("Resources/SoundEffects/{}.wav", title);
	sf::SoundBuffer buffer;
	if (buffer.loadFromFile(path.c_str()))
	{
		this->s_ResourceManager.add<ResourceManager::AudioObject>(title);
		this->s_ResourceManager.getUniquePtr<ResourceManager::AudioObject>(title)->Buffer = std::move(buffer);
		this->m_SoundEffect = this->s_ResourceManager.get<ResourceManager::AudioObject>(title);
		this->m_SoundEffect->Sound.setBuffer(this->m_SoundEffect->Buffer);
		this->m_SoundEffect->Sound.setLoop(this->m_AudioSettings.m_loop);
		this->m_SoundEffect->Sound.setVolume(this->m_AudioSettings.m_volume);
		this->m_SoundEffect->Sound.setPosition({ 0.f, 0.f, 0.f });
		this->m_SoundEffect->Sound.setRelativeToListener(true);
		return true;
	}
	return false;
}

SoundEffect3D::SoundEffect3D(const char* title, sf::Vector3f pos, AudioSettings ms) noexcept : m_position(pos)
{
	if (!this->change(title, ms))
		std::printf("Failed to load %s sound effect!", title);
	else
	{
		this->m_SoundEffect->Sound.setRelativeToListener(false);
		this->m_SoundEffect->Sound.setPosition(this->m_position);
	}
}