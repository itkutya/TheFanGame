#include "Floor.h"

#include <random>
#include <cmath>

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

	if (this->m_layout.size())
	{
		std::size_t startPos = 0;
		std::size_t endPos = 0;
		//int distance = std::abs(endPos - startPos);
		while (!this->m_hasStart || !this->m_hasExit)
		{
			std::random_device rd;
			std::uniform_int_distribution<std::size_t>dist(0, this->m_layout.size() - 1);
			if (!this->m_hasStart)
			{
				startPos = dist(rd);
				auto& start = std::get<Wall>(this->m_layout[startPos]);
				if (start.m_TextureType == TileType::TextureType::None)
				{
					start = Wall(TileType::TextureType::Grassy);
					this->m_hasStart = true;
				}
			}
			if (!this->m_hasExit)
			{
				endPos = dist(rd);
				auto& exit = std::get<Wall>(this->m_layout[endPos]);
				if (exit.m_TextureType == TileType::TextureType::None)
				{
					exit = Wall(TileType::TextureType::Grassy);
					this->m_hasExit = true;
				}
			}
		}
	}

	//Remove it later...
	std::printf("Dungeon has been created:\n");
	std::vector<int> temp(x * y);
	for (std::size_t i = 0; i < this->m_layout.size(); ++i)
	{
		auto& [ground, wall, ceiling] = this->m_layout[i];
		if (wall.m_occupied)
		{
			if (wall.m_TextureType == TileType::TextureType::Grassy)
				temp[i] = 2;
			else
				temp[i] = 1;
		}
		std::printf("%i", temp[i]);
		if (i != 0 && (i % x == std::size_t(x - 1)))
			std::printf("\n");
	}
}