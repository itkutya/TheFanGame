#include "Floor.h"

Floor::Floor(const int x, const int y) noexcept
{
	this->m_layout.resize(std::size_t(x * y));
	this->m_layout.reserve(std::size_t(x * y));

	for (auto tile = this->m_layout.begin(); tile != this->m_layout.end(); ++tile)
	{
		auto& [ground, wall, ceiling] = *tile;
		std::size_t i = std::distance(this->m_layout.begin(), tile);
		if (i == 0 || i < x || i % x == 0 || i % x == std::size_t(x - 1) || (i > std::size_t(x * y) - x))
			wall = Wall(TileType::TextureType::Bricky);
	}

	//Remove it later...
	std::vector<int> temp(x * y);
	for (std::size_t i = 0; i < this->m_layout.size(); ++i)
	{
		auto& [ground, wall, ceiling] = this->m_layout[i];
		if (wall.m_occupied)
			temp[i] = 1;
		std::printf("%i", temp[i]);
		if (i != 0 && (i % x == std::size_t(x - 1)))
			std::printf("\n");
	}
}