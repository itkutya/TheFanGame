#include "entity.h"

entity::entity() noexcept {}

entity::~entity() noexcept {}

sf::Vertex& entity::operator[](const std::size_t& index)
{
	if (index > this->m_vertices.getVertexCount())
		throw "Out of bound...\n";
	return this->m_vertices[index];
}

const void entity::setTexture(const std::uint8_t& index) noexcept { this->m_texture = index; }

void entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(this->m_sprites, states.texture);
	target.draw(this->m_vertices, states.transform);
}