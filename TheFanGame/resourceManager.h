#pragma once

#include <unordered_map>
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
	resourceManager() = delete;
	resourceManager(const resourceManager&) = delete;
	virtual ~resourceManager() {};

	static const void addTexture(const std::string& id, const std::string& filePath, const bool& wantRepeated = false);
	static const void addFont(const std::string& id, const std::string& filePath);
	static const void addSoundBuffer(const std::string& id, const std::string& filePath);

	static const sf::Texture& getTexture(const std::string& id);
	static const sf::Font& getFont(const std::string& id);
	static const sf::SoundBuffer& getSoundBuffer(const std::string& id);

	static void clear();
private:
	static std::unordered_map<std::string, sf::Texture> m_textures;
	static std::unordered_map<std::string, sf::Font> m_fonts;
	static std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
};