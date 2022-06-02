#pragma once

#include "player.h"
#include "world.h"

class ray : public entity
{
public:
    ray();
    virtual ~ray();

    const bool castRay(player* player, world* world);
private:
};

