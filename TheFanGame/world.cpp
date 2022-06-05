#include "world.h"

world::world() noexcept
{
	this->m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
    this->m_vertices.resize(static_cast<size_t>(this->mapWidth * this->mapHeight) * 4);

    this->mapSize = { 4, 4 };

    sf::Color mapColor;
    for (unsigned int i = 0; i < this->mapWidth; ++i)
        for (unsigned int j = 0; j < this->mapHeight; ++j)
        {
            if (this->getMapTile(i, j) > 0)
            {
                sf::Vertex* quad = &this->m_vertices[(i + this->mapWidth * static_cast<size_t>(j)) * 4];

                quad[0].position = sf::Vector2f((float)(i * this->mapSize.x), (float)(j * this->mapSize.y));
                quad[1].position = sf::Vector2f(((float)(i + 1) * this->mapSize.x), (float)(j * this->mapSize.y));
                quad[2].position = sf::Vector2f((float)((i + 1) * this->mapSize.x), (float)((j + 1) * this->mapSize.y));
                quad[3].position = sf::Vector2f((float)(i * this->mapSize.x), (float)((j + 1) * this->mapSize.y));

                switch (this->getMapTile(i, j))
                {
                    case 1:  mapColor = sf::Color::Red;    break;
                    case 2:  mapColor = sf::Color::Magenta;break;
                    case 3:  mapColor = sf::Color::White;  break;
                    case 4:  mapColor = sf::Color::Black;  break;
                    default: mapColor = sf::Color::Yellow; break;
                }

                quad[0].color = mapColor;
                quad[1].color = mapColor;
                quad[2].color = mapColor;
                quad[3].color = mapColor;
            }
        }
}

world::~world() noexcept {}

const int world::getMapTile(const int& x, const int& y) const 
{ 
    if (x >= 0 && x <= this->mapWidth && y >= 0 && y <= this->mapHeight)
        return this->worldMap[x + y * this->mapWidth];
    else
        return 0;
}

void world::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &this->m_texture;
    target.draw(this->m_vertices, states);
}
