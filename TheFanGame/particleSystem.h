#pragma once

#include "includes.h"

struct particle
{
	particle(const float& lifeTime) noexcept
	{
		this->m_LifeTime = lifeTime;
	};
	float m_LifeTime;
	sf::CircleShape m_shape;
	sf::Clock m_Clock;
	sf::Vector2f m_Force;
};

class particleEmitter : public sf::Drawable, public sf::Transformable
{
public:
	explicit particleEmitter(const std::size_t& reserve = 0) noexcept;
	virtual ~particleEmitter() noexcept = default;

	const void createParticle(const std::size_t& amount = 1, const float& lifeTime = 1.f, 
							  const sf::Color& color = sf::Color::White, const sf::Vector2f& position = sf::Vector2f(), 
							  const sf::Vector2f& force = sf::Vector2f()) noexcept;
	const void update() noexcept;
	const void addForce(const sf::Vector2f& direction) noexcept;
	const std::size_t getSize() const noexcept;
private:
	std::vector<particle> m_Particles;

	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;
};

