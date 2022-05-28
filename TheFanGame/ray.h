#pragma once

#include "player.h"
#include "world.h"

class ray : public entity
{
public:
    ray();
    virtual ~ray();

    const bool castRay(const player* player, const world* world);
private:
    sf::Vector2f sideDist;
    sf::Vector2i step;
    bool side = false;
    bool hit = false;
    float perpWallDist = 0.f;
};

