#pragma once

#include <map>
#include <memory>
#include <string>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Graphics/Font.hpp"
		#include "SFML64/Graphics/Texture.hpp"
		#include "SFML64/Audio.hpp"
	#else
		#include "SFML32/Graphics/Font.hpp"
		#include "SFML32/Graphics/Texture.hpp"
		#include "SFML32/Audio.hpp"
	#endif
#endif

class resourceManager
{
public:
	resourceManager() noexcept {};
	virtual ~resourceManager() { this->m_textures.clear(); this->m_fonts.clear(); this->m_soundBuffers.clear(); };

	const void addTexture(const std::uint8_t& id, const std::string& filePath, const bool& wantRepeated = false);
	const void addFont(const std::uint8_t& id, const std::string& filePath);
	const void addSoundBuffer(const std::uint8_t& id, const std::string& filePath);

	const sf::Texture& getTexture(const std::uint8_t& id) const;
	const sf::Font& getFont(const std::uint8_t& id) const;
	const sf::SoundBuffer& getSoundBuffer(const std::uint8_t& id) const;
private:
	std::map<const std::uint8_t, std::unique_ptr<sf::Texture>> m_textures;
	std::map<const std::uint8_t, std::unique_ptr<sf::Font>> m_fonts;
	std::map<const std::uint8_t, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
};