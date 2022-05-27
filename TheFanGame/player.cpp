#include "player.h"

player::player(const sf::PrimitiveType& type, const std::size_t& size)
{
	this->m_vertices.setPrimitiveType(type);
	this->m_vertices.resize(size);
}

player::~player() {}
