#pragma once

#include <initializer_list>

#include "Gameplay/Entity.h"
#include "Floor.h"

class Dungeon : public Entity
{
public:
	Dungeon(const std::initializer_list<Floor> floors) noexcept;
	~Dungeon() noexcept = default;

	std::uint32_t m_currentFloor = 0;
	std::vector<Floor> m_floors;
private:
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

	std::vector<sf::VertexBuffer> m_walls;
};