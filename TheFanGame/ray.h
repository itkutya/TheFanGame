#pragma once

#include "player.h"
#include "world.h"

class ray : public sf::Drawable, public sf::Transformable
{
public:
    ray();
    virtual ~ray();

    const void castRay(player* player, world* world, const unsigned int& screenWidth, const unsigned int& screenHeight, unsigned int& i, sf::Vector2f& dir);
    const bool& isHit() const noexcept;
    const bool& isSide() const noexcept;
    const sf::Vector2i& getDraw() const noexcept;
    const sf::Vector2f& getRayDir() const noexcept;
    const sf::Vector2i& getMapPos() const noexcept;
    const float& getDistance() const noexcept;
    const void resize(const unsigned int& screenWidth) noexcept;

    virtual sf::Vertex& operator[] (const std::size_t index);
private:
    sf::Vector2f sideDist;
    sf::Vector2i step;
    sf::Vector2i drawSE;
    sf::Vector2f rayDir;
    sf::Vector2i map;
    bool side;
    bool hit;
    float perpWallDist;

    sf::VertexArray m_vertices;
    sf::Texture m_texture;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

