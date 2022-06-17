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
	entity(const sf::Vector2f& size = sf::Vector2f(0.f, 0.f), const sf::Vector2f& pos = sf::Vector2f(0.f, 0.f), const sf::Color& color = sf::Color::White) noexcept;
	virtual ~entity() noexcept;

	virtual sf::Vertex& operator[] (const std::size_t& index);
	const void setSize(const sf::IntRect& size) noexcept;

	sf::VertexArray m_vertices;
	sf::Vector2f m_direction;
	sf::Vector2f m_plane;
	float m_angle;
	std::uint8_t m_texture;
	sf::Vector3f m_size; //|| x = uDiv || y = vDiv || z = vMove ||
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};