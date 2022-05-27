#include "entity.h"

entity::entity(const sf::PrimitiveType& type, const std::size_t& size) {
	this->m_vertices.setPrimitiveType(type);
	this->m_vertices.resize(size);
}

entity::~entity() noexcept {}

sf::Vertex& entity::operator[](const std::size_t index) {
	if (index > this->m_vertices.getVertexCount())
		throw "Out of bound...\n";
	return this->m_vertices[index];
}

void entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = &this->m_texture;
	target.draw(this->m_vertices, states);
}
