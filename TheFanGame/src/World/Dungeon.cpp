#include "Dungeon.h"

Dungeon::Dungeon(const std::initializer_list<Floor> floors) noexcept
{
	this->m_floors.reserve(floors.size());
	for (auto& floor : floors)
	{
		this->m_floors.emplace_back(floor);
		auto& wall = this->m_walls.emplace_back(sf::PrimitiveType::Lines, sf::VertexBuffer::Usage::Static);
		if (!wall.create((std::size_t)floor.m_size.x * (std::size_t)floor.m_size.y * 8))
			std::printf("Failed to create vertex buffer...\n");
	}

	for (std::size_t f = 0; f < this->m_floors.size(); ++f)
	{
		for (std::size_t i = 0; i < this->m_floors[f].m_size.x; ++i)
		{
			for (std::size_t j = 0; j < this->m_floors[f].m_size.y; ++j)
			{
				float offset = 12.f;
				sf::Vertex quad[8];

				quad[0].position = { i * 50.f + offset, j * 50.f + offset };
				quad[1].position = { i * 50.f + offset + 50.f, j * 50.f + offset };

				quad[2].position = { i * 50.f + offset + 50.f, j * 50.f + offset };
				quad[3].position = { i * 50.f + offset + 50.f, j * 50.f + offset + 50.f };

				quad[4].position = { i * 50.f + offset + 50.f, j * 50.f + offset + 50.f };
				quad[5].position = { i * 50.f + offset, j * 50.f + offset + 50.f };

				quad[6].position = { i * 50.f + offset, j * 50.f + offset + 50.f };
				quad[7].position = { i * 50.f + offset, j * 50.f + offset };

				if (std::get<Wall>(this->m_floors[f].m_layout[j + i * this->m_floors[f].m_size.x]).m_TextureType != TileType::TextureType::Bricky)
				{
					quad[0].color = sf::Color::Transparent;
					quad[1].color = sf::Color::Transparent;
					quad[2].color = sf::Color::Transparent;
					quad[3].color = sf::Color::Transparent;
					quad[4].color = sf::Color::Transparent;
					quad[5].color = sf::Color::Transparent;
					quad[6].color = sf::Color::Transparent;
					quad[7].color = sf::Color::Transparent;
				}

				if (!this->m_walls[f].update(quad, 8, ((std::uint32_t)i + (std::uint32_t)j * this->m_floors[f].m_size.x) * 8))
					std::printf("Failed to update vertex buffer...\n");
			}
		}
	}
}

void Dungeon::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
	if (this->m_walls.size())
		target.draw(this->m_walls[this->m_currentFloor], states);
}