#pragma once

#include "entity.h"
#include "ray.h"

class player : public entity
{
public:
	player(const sf::PrimitiveType& type = sf::PrimitiveType::Quads, const std::size_t& size = 4);
	virtual ~player();
private:
	sf::Vector2f plane;
	float angle = 0.f;
};

