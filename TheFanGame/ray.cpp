#include "ray.h"

ray::ray()
{
	this->m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
	this->m_vertices.resize(601); //TODO: Window width problem fix later...
}

ray::~ray()
{
}

const float ray::castRay(player* player, world* world, const unsigned int& screenWidth, unsigned int& i, sf::Vector2f& dir)
{
    sideDist = sf::Vector2f();
    step = sf::Vector2i();
    side = false;
    hit = false;

    float cameraX = 2 * (i - 1) / (float)screenWidth - 1;
    sf::Vector2f rayDir;
    rayDir.x = dir.x + (-dir.y / 3.f) * cameraX;
    rayDir.y = dir.y + (dir.x / 3.f) * cameraX;

    sf::Vector2i map;
    map.x = int(player->getPosition().x / world->mapSize.x);
    map.y = int(player->getPosition().y / world->mapSize.y);

    sf::Vector2f deltaDist;
    deltaDist.x = (rayDir.x == 0.f) ? 1e30f : std::abs(1.f / rayDir.x);
    deltaDist.y = (rayDir.y == 0.f) ? 1e30f : std::abs(1.f / rayDir.y);

    if (rayDir.x < 0)
    {
        this->step.x = -1;
        this->sideDist.x = (player->getPosition().x / world->mapSize.x - map.x) * deltaDist.x;
    }
    else
    {
        this->step.x = 1;
        this->sideDist.x = (map.x + 1.0f - player->getPosition().x / world->mapSize.x) * deltaDist.x;
    }
    if (rayDir.y < 0)
    {
        this->step.y = -1;
        this->sideDist.y = (player->getPosition().y / world->mapSize.y - map.y) * deltaDist.y;
    }
    else
    {
        this->step.y = 1;
        this->sideDist.y = (map.y + 1.0f - player->getPosition().y / world->mapSize.y) * deltaDist.y;
    }

    float fDistance = 0.f;
    while (!this->hit && fDistance < 10.f)
    {
        if (this->sideDist.x < this->sideDist.y)
        {
            fDistance = this->sideDist.x;
            this->sideDist.x += deltaDist.x;
            map.x += this->step.x;
            this->side = false;
        }
        else
        {
            fDistance = this->sideDist.y;
            this->sideDist.y += deltaDist.y;
            map.y += this->step.y;
            this->side = true;
        }
        if (map.x >= 0 && map.x <= world->mapWidth && map.y >= 0 && map.y <= world->mapHeight)
            if (world->getMapTile(map.x, map.y) > 0)
                this->hit = true;
    }

    if (fDistance > 10.f)
        fDistance = 10.f;

    this->m_vertices[i].position = sf::Vector2f(player->getPosition().x + rayDir.x * fDistance * world->mapSize.x,
                                                player->getPosition().y + rayDir.y * fDistance * world->mapSize.y);
    return fDistance;
}

const bool ray::isHit() const noexcept { return this->hit; }

/*
        int lineHeight = (int)(screenHeight / perpWallDist);
        int drawStart = (int)(-lineHeight / 2 + screenHeight / 2 * player->angle);
        int drawEnd = (int)(lineHeight / 2 + screenHeight / 2 * angle);

        color = sf::Color(255 - (int)(perpWallDist * 5 > 255 ? 255 : perpWallDist * 5),
            255 - (int)(perpWallDist * 5 > 255 ? 255 : perpWallDist * 5),
            255 - (int)(perpWallDist * 5 > 255 ? 255 : perpWallDist * 5), 255);

        if (side == 1) { color.r = sf::Uint8(color.r / 1.5f); color.g = sf::Uint8(color.g / 1.5f); color.b = sf::Uint8(color.b / 1.5f); }

        sf::Vertex* walls = &line[i * 2];
        walls[0].position = sf::Vector2f((float)i + 1, (float)drawStart);
        walls[1].position = sf::Vector2f((float)i + 1, (float)drawEnd);

        walls[0].color = color;
        walls[1].color = color;

        float wallX;
        if (side == 0) wallX = player->getPosition().y + perpWallDist * rayDir.y;
        else           wallX = player->getPosition().x + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        int texX = int(wallX * double(texWidth));
        if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
        if (side == 1 && rayDir.y < 0) texX = texWidth - texX - 1;

        int mapNum = worldMap[map.x][mapY];
        walls[0].texCoords = sf::Vector2f(texX + (texWidth * mapNum + 0.5f), 0.f);
        walls[1].texCoords = sf::Vector2f(texX + (texWidth * mapNum + 0.5f), (float)texHeight);

        ZBuffer[i] = perpWallDist;
*/