#include "particleSystem.h"

particleEmitter::particleEmitter(const std::size_t& reserve) noexcept
{
	this->m_Particles.reserve(reserve);
}

const void particleEmitter::createParticle(const std::size_t& amount, const float& lifeTime, const sf::Color& color, const sf::Vector2f& position, const sf::Vector2f& force) noexcept
{
	for (std::size_t i = 0; i < amount; i++)
	{
		this->m_Particles.emplace_back(lifeTime);
		auto& newParticle = (*(this->m_Particles.end() - 1));
		newParticle.m_shape.setRadius(10.f);
		newParticle.m_shape.setFillColor(color);
		newParticle.m_shape.setPosition(position);
		newParticle.m_Force = force;
	}
}

const void particleEmitter::update() noexcept
{
	this->m_Particles.erase(
		std::remove_if(
			this->m_Particles.begin(),
			this->m_Particles.end(),
			[](particle const& p) { return p.m_Clock.getElapsedTime().asSeconds() >= p.m_LifeTime; }
		),
		this->m_Particles.end()
	);
	for (auto it = this->m_Particles.begin(); it != this->m_Particles.end(); ++it)
	{
		(*it).m_shape.move((*it).m_Force);
		(*it).m_shape.setRadius(10.f * ((*it).m_LifeTime / ((*it).m_Clock.getElapsedTime().asSeconds() + (*it).m_LifeTime)));
	}
}

const void particleEmitter::addForce(const sf::Vector2f& direction) noexcept
{
	for (auto& particle : this->m_Particles)
		particle.m_Force = direction;
}

const std::size_t particleEmitter::getSize() const noexcept
{
	return this->m_Particles.size();
}

void particleEmitter::draw(sf::RenderTarget& target, const sf::RenderStates& states) const
{
	for (auto& particle : this->m_Particles)
		target.draw(particle.m_shape);
}
