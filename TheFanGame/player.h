#pragma once

#include "entity.h"

class player : public entity
{
public:
	player(const sf::PrimitiveType& type = sf::PrimitiveType::Quads, const std::size_t& size = 4);
	virtual ~player();
private:
};

