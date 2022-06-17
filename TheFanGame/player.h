#pragma once

#include "entity.h"

class player : public entity
{
public:
	player(const sf::Vector2f& size = sf::Vector2f(0.f, 0.f), const sf::Vector2f& pos = sf::Vector2f(0.f, 0.f), const sf::Color& color = sf::Color::White) noexcept;
	virtual ~player() noexcept;

	virtual const void update(const sf::Vector2i& mousePos, const sf::Vector2u& windowSize, const sf::Time& dt) noexcept override;
	virtual const void setSize(const sf::IntRect& size) noexcept override;

	inline virtual const void update(entity& player, const sf::Vector2u& windowSize, const std::vector<float>& zBuffer) noexcept override {};
private:
	float movementSpeed;
	float mouseSensivity;
};