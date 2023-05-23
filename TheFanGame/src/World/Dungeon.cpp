#include "Dungeon.h"

Dungeon::Dungeon(const int maxlvl) noexcept
{
	this->m_floors.resize(maxlvl);
	this->m_floors.reserve(maxlvl);
}