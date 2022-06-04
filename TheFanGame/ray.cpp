#include "ray.h"

ray::ray()
{
	this->m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
	this->m_vertices.resize(601); //TODO: Window width problem fix later...

    this->side = false;
    this->hit = false;
    this->perpWallDist = 0.f;
}

ray::~ray()
{
}

const sf::Vector2i ray::castRay(player* player, world* world, const unsigned int& screenWidth, const unsigned int& screenHeight, unsigned int& i, sf::Vector2f& dir)
{
    this->sideDist = sf::Vector2f();
    this->step = sf::Vector2i();
    this->side = false;
    this->hit = false;
    this->perpWallDist = 0.f;

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

    int lineHeight = (int)(screenHeight / this->perpWallDist);
    sf::Vector2i draw;
    draw.x = (int)(-lineHeight / 2 + screenHeight / 2 * player->angle);
    draw.y = (int)(lineHeight / 2 + screenHeight / 2 * player->angle);

    if (this->side == 1) 
    { 
        this->m_vertices[i].color.r = sf::Uint8(this->m_vertices[i].color.r / 1.5f);
        this->m_vertices[i].color.g = sf::Uint8(this->m_vertices[i].color.g / 1.5f);
        this->m_vertices[i].color.b = sf::Uint8(this->m_vertices[i].color.b / 1.5f);
    }

    //float wallX;
    //if (side == 0) wallX = player->getPosition().y + perpWallDist * rayDir.y;
    //else           wallX = player->getPosition().x + perpWallDist * rayDir.x;
    //wallX -= floor((wallX));

    //int texX = int(wallX * double(texWidth));
    //if (side == 0 && rayDir.x > 0) texX = texWidth - texX - 1;
    //if (side == 1 && rayDir.y < 0) texX = texWidth - texX - 1;

    //int mapNum = world->getMapTile(map.x, map.y);
    //walls[0].texCoords = sf::Vector2f(texX + (texWidth * mapNum + 0.5f), 0.f);
    //walls[1].texCoords = sf::Vector2f(texX + (texWidth * mapNum + 0.5f), (float)texHeight);

    //ZBuffer[i] = perpWallDist;

    return draw;
}

const bool ray::isHit() const noexcept { return this->hit; }

sf::Vertex& ray::operator[](const std::size_t index)
{
    if (index > this->m_vertices.getVertexCount())
        throw "Out of bound...\n";
    return this->m_vertices[index];
}

void ray::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &this->m_texture;
    target.draw(this->m_vertices, states);
}