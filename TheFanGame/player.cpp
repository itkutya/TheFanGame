#include "player.h"

player::player(const sf::PrimitiveType& type, const std::size_t& size)
{
	this->m_vertices.setPrimitiveType(type);
	this->m_vertices.resize(size);

	this->m_vertices[0].position = sf::Vector2f(-6, -6);
	this->m_vertices[1].position = sf::Vector2f(6, -6);
	this->m_vertices[2].position = sf::Vector2f(6, 6);
	this->m_vertices[3].position = sf::Vector2f(-6, 6);

	this->m_vertices[0].color = sf::Color::Cyan;
	this->m_vertices[1].color = sf::Color::Cyan;
	this->m_vertices[2].color = sf::Color::Cyan;
	this->m_vertices[3].color = sf::Color::Cyan;

	this->direction = { -1.f, 0.f };
	this->plane = { 0.f, 0.3f };
	this->angle = 0.f;
}

player::~player() {}

const void player::setSize(const sf::IntRect& size) noexcept
{
	this->m_vertices[0].position = sf::Vector2f((float)size.left, (float)size.top);
	this->m_vertices[1].position = sf::Vector2f((float)size.width, (float)size.top);
	this->m_vertices[2].position = sf::Vector2f((float)size.width, (float)size.height);
	this->m_vertices[3].position = sf::Vector2f((float)size.left, (float)size.height);
}
