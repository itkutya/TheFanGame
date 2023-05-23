#pragma once

#include <tuple>

#include "Managers/ResourceManager.h"

constexpr std::uint16_t TILE_TEXTURE_WIDTH = 16;
constexpr std::uint16_t TILE_TEXTURE_HEIGHT = 16;

struct TileType
{
public:
	enum TextureType
	{
		None, Grassy, Bricky
	};
	TextureType m_TextureType = TextureType::None;
	sf::Texture* m_texture = nullptr;
	bool m_occupied = false;
protected:
	virtual void construct(const char* tile, TextureType type = TextureType::None) noexcept;
private:
	ResourceManager& s_ResourceManager = ResourceManager::getInstance();
};

struct Ground : public TileType
{
	explicit Ground(TextureType type = TextureType::None) noexcept
	{
		this->construct("Ground", type);
	};

	Ground operator=(const Ground& other) noexcept
	{
		this->m_occupied = other.m_occupied;
		this->m_TextureType = other.m_TextureType;
		this->m_texture = other.m_texture;
		return *this;
	};
};

struct Wall : public TileType
{
	explicit Wall(TextureType type = TextureType::None) noexcept
	{
		this->construct("Wall", type);
	};

	Wall operator=(const Wall& other) noexcept
	{
		this->m_occupied = other.m_occupied;
		this->m_TextureType = other.m_TextureType;
		this->m_texture = other.m_texture;
		return *this;
	};
};

struct Ceiling : public TileType
{
	explicit Ceiling(TextureType type = TextureType::None) noexcept
	{
		this->construct("Ceiling", type);
	};

	Ceiling operator=(const Ceiling& other) noexcept
	{
		this->m_occupied = other.m_occupied;
		this->m_TextureType = other.m_TextureType;
		this->m_texture = other.m_texture;
		return *this;
	};
};