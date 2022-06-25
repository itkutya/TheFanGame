#pragma once

#include "player.h"
#include "world.h"

class ray : public sf::Drawable, public sf::Transformable
{
public:
    ray(const std::uint32_t& screenWidth);
    virtual ~ray();

    const void castRay(player& player, world& world, const sf::Vector2u& screenSize, const std::uint32_t& i);
    
    float perpWallDist;
    sf::VertexArray r_vertices;
    sf::VertexArray r_walls;
    sf::VertexArray r_floors;
    sf::VertexArray r_ceilings;
private:
    sf::Vector2f r_rayDir;
    sf::Vector2i r_stepSize;
    sf::Vector2i r_mapPos;
    bool side;
    bool hit;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

