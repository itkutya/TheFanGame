#pragma once

#include "player.h"

class quad : public entity
{
public:
	quad(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Color& color = sf::Color::Magenta) noexcept;
	virtual ~quad() noexcept;
private:
};