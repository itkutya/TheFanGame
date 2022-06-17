#pragma once

#include "player.h"
#include "world.h"

class ray : public sf::Drawable, public sf::Transformable
{
public:
    ray(const std::uint32_t& screenWidth);
    virtual ~ray();

    const void castRay(player& player, world& world, const std::uint32_t& screenWidth, const std::uint32_t& screenHeight, const std::uint32_t& i);
    sf::Vertex& operator[] (const std::size_t& index);
    
    float perpWallDist;
    sf::VertexArray r_vertices;
    sf::VertexArray r_walls;
private:
    sf::Vector2f r_rayDir;
    sf::Vector2i r_stepSize;
    sf::Vector2i r_mapPos;
    bool side;
    bool hit;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

