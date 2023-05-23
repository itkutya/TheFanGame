#include "Tile.h"

#include <format>

void TileType::construct(const char* tile, TextureType type) noexcept
{
	this->m_TextureType = type;
	switch (this->m_TextureType)
	{
	case TextureType::Grassy:
		this->m_texture = this->s_ResourceManager.add<sf::Texture>(std::format("Grassy{}Texture", tile).c_str());
		this->m_occupied = true;
		break;
	case TextureType::Bricky:
		this->m_texture = this->s_ResourceManager.add<sf::Texture>(std::format("Bricky{}Texture", tile).c_str());
		this->m_occupied = true;
		break;
	}
	if (this->m_texture && !this->m_texture->loadFromFile(std::format("Resources/Textures/{}Textures.png", tile).c_str(), sf::IntRect({ this->m_TextureType * TILE_TEXTURE_WIDTH, 0 }, { TILE_TEXTURE_WIDTH, TILE_TEXTURE_HEIGHT })))
		std::printf("Failed to load in %s's texture!\n", tile);
}