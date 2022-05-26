#include "player.h"

player::player(const sf::PrimitiveType& type, const std::size_t& size, const std::string& source)
{
	this->m_vertices.setPrimitiveType(type);
	this->m_vertices.resize(size);

	if (!this->m_texture.loadFromFile(source))
		throw "Cannot load texture...\n";
}

player::~player() {}
