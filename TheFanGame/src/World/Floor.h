#pragma once

#include <vector>

#include "Tile.h"

class Floor
{
public:
	Floor(const int x = 0, const int y = 0) noexcept;
	~Floor() noexcept = default;
private:
	bool hasBoss = false;
	bool isSafePoint = false;
	//...
	std::vector<Tile> m_layout;
};