#pragma once

#include <math.h>

#include "resourceManager.h"
#include "animation.h"
#include "world.h"

constexpr float PI = 3.141592653589793116f;

class entity : public sf::Drawable, public sf::Transformable
{
public:
	entity() noexcept;
	virtual ~entity() noexcept;

	virtual sf::Vertex& operator[] (const std::size_t& index);
	virtual const void setTexture(const std::uint8_t& index) noexcept;
	virtual const void setSize(const sf::IntRect& size) noexcept = 0;
	virtual const void update(entity& player, const sf::Vector2u& windowSize, const std::vector<float>& zBuffer) noexcept = 0;
	virtual const void update(world& map, const sf::Vector2i& mousePos, const sf::Vector2u& windowSize, const sf::Time& dt) noexcept = 0;

	sf::VertexArray m_vertices;
	sf::VertexArray m_sprites;
	sf::Vector2f m_direction = sf::Vector2f(0.f, 0.f);
	sf::Vector2f m_plane = sf::Vector2f(0.f, 0.f);
	float m_angle = 0.f;
	std::uint8_t m_texture = 0;
	sf::Vector3f m_size = sf::Vector3f(0.f, 0.f, 0.f);
	//|| x = uDiv || y = vDiv || z = vMove ||
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};