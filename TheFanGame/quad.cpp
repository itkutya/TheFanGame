#include "quad.h"

quad::quad(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Color& color) noexcept
{
	this->m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
	this->m_vertices.resize(4);

	this->m_vertices[0].position = sf::Vector2f(-size.x, -size.y);
	this->m_vertices[1].position = sf::Vector2f(size.x, -size.y);
	this->m_vertices[2].position = sf::Vector2f(size.x, size.y);
	this->m_vertices[3].position = sf::Vector2f(-size.x, size.y);

	this->setPosition(pos);

	this->m_vertices[0].color = color;
	this->m_vertices[1].color = color;
	this->m_vertices[2].color = color;
	this->m_vertices[3].color = color;
}

quad::~quad() noexcept {}