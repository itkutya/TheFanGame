#pragma once

#include <vector>

#include "Tile.h"

class Floor
{
	typedef std::tuple<Ground, Wall, Ceiling> Tile;
public:
	enum class FloorType
	{
		None, Randomized, NotRandomized
	};

	Floor(const std::uint32_t x, const std::uint32_t y, const FloorType type = FloorType::None) noexcept;
	~Floor() noexcept = default;

	bool hasBoss = false;
	bool isSafePoint = false;
	bool m_hasStart = false;
	bool m_hasExit = false;
	sf::Vector2u m_size;
	FloorType m_floorType;
	//...
	std::vector<Tile> m_layout;
private:
};