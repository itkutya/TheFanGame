#pragma once

#include "player.h"
#include "world.h"

class ray : public entity
{
public:
    ray();
    virtual ~ray();

    const bool castRay(player* player, world* world, const unsigned int& screenWidth, unsigned int& i, sf::Vector2f& dir);
private:
};

