#include "entity.h"

entity::entity(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Color& color) noexcept 
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

	this->m_direction = { -1.f, 0.f };
	this->m_plane = { 0.f, 0.3f };
	this->m_angle = 0.f;
	this->m_texture = 4;
	this->m_size = { 0.5f, 0.5f, 0.f };
}

entity::~entity() noexcept {}

sf::Vertex& entity::operator[](const std::size_t index) 
{
	if (index > this->m_vertices.getVertexCount())
		throw "Out of bound...\n";
	return this->m_vertices[index];
}

const void entity::setSize(const sf::IntRect& size) noexcept
{
	this->m_vertices[0].position = sf::Vector2f((float)size.left, (float)size.top);
	this->m_vertices[1].position = sf::Vector2f((float)size.width, (float)size.top);
	this->m_vertices[2].position = sf::Vector2f((float)size.width, (float)size.height);
	this->m_vertices[3].position = sf::Vector2f((float)size.left, (float)size.height);
}

void entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(this->m_vertices, states);
}