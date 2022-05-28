#include "player.h"

player::player(const sf::PrimitiveType& type, const std::size_t& size)
{
	this->m_vertices.setPrimitiveType(type);
	this->m_vertices.resize(size);

	this->m_vertices[0].position = sf::Vector2f(0, 0);
	this->m_vertices[1].position = sf::Vector2f(24, 0);
	this->m_vertices[2].position = sf::Vector2f(24, 24);
	this->m_vertices[3].position = sf::Vector2f(0, 24);

	this->m_vertices[0].color = sf::Color::Cyan;
	this->m_vertices[1].color = sf::Color::Cyan;
	this->m_vertices[2].color = sf::Color::Cyan;
	this->m_vertices[3].color = sf::Color::Cyan;
}

player::~player() {}
