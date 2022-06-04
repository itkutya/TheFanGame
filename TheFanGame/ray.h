#pragma once

#include "player.h"
#include "world.h"

class ray : public sf::Drawable, public sf::Transformable
{
public:
    ray();
    virtual ~ray();

    const sf::Vector2i castRay(player* player, world* world, const unsigned int& screenWidth, const unsigned int& screenHeight, unsigned int& i, sf::Vector2f& dir);
    const bool isHit() const noexcept;

    virtual sf::Vertex& operator[] (const std::size_t index);
private:
    sf::Vector2f sideDist;
    sf::Vector2i step;
    bool side;
    bool hit;
    float perpWallDist;

    sf::VertexArray m_vertices;
    sf::Texture m_texture;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

