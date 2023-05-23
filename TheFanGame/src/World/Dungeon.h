#pragma once

#include "Floor.h"

class Dungeon
{
public:
	Dungeon(const int maxlvl = 0) noexcept;
	~Dungeon() noexcept = default;
private:
	std::vector<Floor> m_floors;
};