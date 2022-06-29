#pragma once

#include <math.h>

#include "resourceManager.h"
#include "animation.h"
#include "world.h"

#include "ImGUI/imgui.h"

constexpr float PI = 3.141592653589793116f;

class entity : public sf::Drawable, public sf::Transformable
{
public:
	entity(const sf::Vector2f& size = sf::Vector2f(), const sf::Vector2f& pos = sf::Vector2f(), const sf::Color& color = sf::Color::White) noexcept;
	virtual ~entity() noexcept;
	
	const void update(entity& player, const sf::Vector2u& windowSize, const std::vector<float>& zBuffer) noexcept;

	virtual sf::Vertex& operator[] (const std::size_t& index);
	virtual const void setTexture(const std::uint8_t& index) noexcept;
	virtual const void setSize(const sf::IntRect& size) noexcept;

	const sf::Color getDistanceColor(const sf::Vector2f& pos);

	sf::VertexArray m_vertices;
	sf::VertexArray m_sprites;
	sf::Vector2f m_direction = sf::Vector2f(0.f, 0.f);
	sf::Vector2f m_plane = sf::Vector2f(0.f, 0.f);
	float m_angle = 0.f;
	std::uint8_t m_texture = 0;
	sf::Vector3f m_size = sf::Vector3f(0.f, 0.f, 0.f);
	//|| x = uDiv || y = vDiv || z = vMove ||

	float health = 100.f;
	animation m_animation;
	float animationSpeed = 0.5f;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};