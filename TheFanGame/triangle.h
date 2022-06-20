#pragma once

#include "entity.h"

class triangle : public entity
{
public:
	triangle(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Color& color = sf::Color::Magenta) noexcept;
	virtual ~triangle() noexcept;

	virtual const void update(entity& player, const sf::Vector2u& windowSize, const std::vector<float>& zBuffer) noexcept override;
	virtual const void setSize(const sf::IntRect& size) noexcept override;

	inline virtual const void update(world& map, const sf::Vector2i& mousePos, const sf::Vector2u& windowSize, const sf::Time& dt) noexcept override {};
private:
};