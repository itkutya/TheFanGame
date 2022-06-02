#include "world.h"

world::world() noexcept
{
	this->m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
	this->m_vertices.resize(mapWidth* mapHeight * 4);

    sf::Color mapColor;
    for (unsigned int i = 0; i < mapWidth; ++i)
        for (unsigned int j = 0; j < mapHeight; ++j)
        {
            if (this->getMapTile(i, j) > 0)
            {
                sf::Vertex* quad = &this->m_vertices[(i + mapWidth * j) * 4];

                quad[0].position = sf::Vector2f((float)(i * this->mapSize.x), (float)(j * mapSize.y));
                quad[1].position = sf::Vector2f(((float)(i + 1) * this->mapSize.x), (float)(j * mapSize.y));
                quad[2].position = sf::Vector2f((float)((i + 1) * this->mapSize.x), (float)((j + 1) * mapSize.y));
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

const int world::getMapTile(const int& x, const int& y) const noexcept { return this->worldMap[x + y * this->mapDimension.x]; }