#pragma once

#include <math.h>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Graphics.hpp"
	#else
		#include "SFML32/Graphics.hpp"
	#endif
#endif

#include "resourceManager.h"
#include "animation.h"

constexpr float PI = 3.141592653589793116f;

class entity : public sf::Drawable, public sf::Transformable
{
public:
	entity(const sf::PrimitiveType& type = sf::PrimitiveType::Quads, const std::size_t& size = 4);
	virtual ~entity() noexcept;

	virtual sf::Vertex& operator[] (const std::size_t index);

	sf::VertexArray m_vertices;
	sf::Vector2f direction;
	sf::Vector2f plane;
	float angle;
	float uDiv;
	float vDiv;
	float vMove;
private:
	sf::Texture m_texture;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

