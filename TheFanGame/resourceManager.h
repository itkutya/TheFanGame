#pragma once

#include <map>
#include <memory>
#include <string>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Graphics/Font.hpp"
		#include "SFML64/Graphics/Texture.hpp"
	#else
		#include "SFML32/Graphics/Font.hpp"
		#include "SFML32/Graphics/Texture.hpp"
	#endif
#endif

class resourceManager
{
public:
	resourceManager() noexcept {};
	virtual ~resourceManager() { this->m_textures.clear(); this->m_fonts.clear(); };

	const void addTexture(const int& id, const std::string& filePath, const bool& wantRepeated = false);
	const void addFont(const int& id, const std::string& filePath);

	const sf::Texture& getTexture(const int& id) const;
	const sf::Font& getFont(const int& id) const;
private:
	std::map<const int, std::unique_ptr<sf::Texture>> m_textures;
	std::map<const int, std::unique_ptr<sf::Font>> m_fonts;
};