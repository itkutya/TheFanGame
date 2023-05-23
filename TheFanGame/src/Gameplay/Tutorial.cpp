#include "Tutorial.h"

void Tutorial::init(sf::RenderWindow& window)
{
	this->m_dungeon = Dungeon(3);
	this->m_dungeon.m_floors[0] = Floor(5, 5);
	this->m_dungeon.m_floors[1] = Floor(12, 12);
	this->m_dungeon.m_floors[2] = Floor(24, 24);
}

void Tutorial::processEvent(sf::Event& event) noexcept
{
}

void Tutorial::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
}

void Tutorial::draw(sf::RenderWindow& window) noexcept
{
}