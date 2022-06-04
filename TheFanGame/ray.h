#pragma once

#include "player.h"
#include "world.h"

class ray : public entity
{
public:
    ray();
    virtual ~ray();

    const float castRay(player* player, world* world, const unsigned int& screenWidth, unsigned int& i, sf::Vector2f& dir);
    const bool isHit() const noexcept;
private:
    sf::Vector2f sideDist;
    sf::Vector2i step;
    bool side = false;
    bool hit = false;
};

