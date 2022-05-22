#pragma once

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Graphics.hpp"
	#else
		#include "SFML32/Graphics.hpp"
	#endif
#endif

class entity : public sf::Drawable, public sf::Transformable
{
public:
	entity(const sf::PrimitiveType& type, const std::size_t& size, const sf::Texture& texture = sf::Texture());
	virtual ~entity();

	virtual operator const sf::Drawable*();
	virtual sf::Vertex& operator[](const std::size_t index);
private:
	sf::VertexArray m_vertices;
	sf::Texture m_texture;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

