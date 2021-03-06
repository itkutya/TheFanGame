#pragma once

#include "weapon.h"

class player : public entity
{
public:
	player(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Color& color = sf::Color::White) noexcept;
	virtual ~player() noexcept;

	virtual const void setSize(const sf::IntRect& size) noexcept override;

	const void update(world& map, const sf::Vector2i& mousePos, const sf::Vector2u& windowSize, std::vector<entity>& entities, const sf::Time& dt) noexcept;
	
	weapon ak47 = weapon(20.f, 31, sf::seconds(0.05f), 10.f, 1.5f, false);
private:
	float movementSpeed;
	float mouseSensivity;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};