#pragma once

#include "weapon.h"

class player : public entity
{
public:
	player(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Color& color = sf::Color::White) noexcept;
	virtual ~player() noexcept;

	virtual const void update(world& map, const sf::Vector2i& mousePos, const sf::Vector2u& windowSize, const sf::Time& dt) noexcept override;
	virtual const void setSize(const sf::IntRect& size) noexcept override;

	inline virtual const void update(entity& player, const sf::Vector2u& windowSize, const std::vector<float>& zBuffer) noexcept override {};
private:
	float movementSpeed;
	float mouseSensivity;

	weapon ak47 = weapon(20.f, 31, sf::seconds(0.05f), 5.f, false);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};