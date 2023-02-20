#pragma once

#include "includes.h"

struct particle
{
	particle(const float& lifeTime, const sf::Color& color, const sf::Vector2f& position, const sf::Vector2f& force) noexcept
	{
		this->m_LifeTime = lifeTime;
		this->m_Color = color;
		this->m_Position = position;
		this->m_Force = force;
	};
	sf::Vertex m_Vertecies[4];
	float m_LifeTime;
	float m_radius = 100.f;
	sf::Vector2f m_Position;
	sf::Color m_Color;
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
	const void update(const sf::Time& dt) noexcept;
	const void addForce(const sf::Vector2f& direction) noexcept;
	const std::size_t getSize() const noexcept;

	const void draw(sf::RenderTarget& target) noexcept;
private:
	std::vector<particle> m_Particles;
	sf::VertexBuffer m_vertexBuffer;
	sf::Shader m_ParticleShader;
	sf::RenderTexture m_ParticleTexture;

	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;
};

