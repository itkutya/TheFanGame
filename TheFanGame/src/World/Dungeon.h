#pragma once

#include "Floor.h"

class Dungeon
{
public:
	Dungeon(const int maxlvl = 0) noexcept;
	~Dungeon() noexcept = default;
	std::vector<Floor> m_floors;
private:
};