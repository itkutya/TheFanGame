#include "ray.h"

ray::ray(const std::uint32_t& screenWidth)
{
	this->r_vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
    this->r_vertices.resize(static_cast<std::size_t>(screenWidth) + 1);

    this->r_walls.setPrimitiveType(sf::PrimitiveType::Lines);
    this->r_walls.resize((static_cast<std::size_t>(screenWidth)) * 2);

    this->r_rayDir = sf::Vector2f(0.f, 0.f);
    this->r_stepSize = sf::Vector2i(0, 0);
    this->r_mapPos = sf::Vector2i(0, 0);
    this->side = false;
    this->hit = false;
    this->perpWallDist = 0.f;
}

ray::~ray() {}

const void ray::castRay(player& player, world& world, const std::uint32_t& screenWidth, const std::uint32_t& screenHeight, std::uint32_t& i)
{
    if (this->r_vertices.getVertexCount() != static_cast<std::size_t>(screenWidth) + 1)
        this->r_vertices.resize(static_cast<std::size_t>(screenWidth) + 1);

    if (this->r_walls.getVertexCount() != static_cast<std::size_t>(screenWidth) * 2)
        this->r_walls.resize((static_cast<std::size_t>(screenWidth)) * 2);

    this->r_rayDir = sf::Vector2f(0.f, 0.f);
    this->r_stepSize = sf::Vector2i(0, 0);
    this->r_mapPos = sf::Vector2i(0, 0);
    this->side = false;
    this->hit = false;
    this->perpWallDist = 0.f;

    float cameraX = (float)(2.f * i / screenWidth - 1.f);
    this->r_rayDir.x = player.m_direction.x + player.m_plane.x * cameraX;
    this->r_rayDir.y = player.m_direction.y + player.m_plane.y * cameraX;

    this->r_mapPos.x = (int)(player.getPosition().x / world.mapSize.x);
    this->r_mapPos.y = (int)(player.getPosition().y / world.mapSize.y);

    sf::Vector2f deltaDist;
    deltaDist.x = (this->r_rayDir.x == 0.f) ? 1e30f : std::abs(1.f / this->r_rayDir.x);
    deltaDist.y = (this->r_rayDir.y == 0.f) ? 1e30f : std::abs(1.f / this->r_rayDir.y);

    sf::Vector2f sideDist;
    if (this->r_rayDir.x < 0.f)
    {
        this->r_stepSize.x = -1;
        sideDist.x = ((player.getPosition().x / world.mapSize.x) - this->r_mapPos.x) * deltaDist.x;
    }
    else
    {
        this->r_stepSize.x = 1;
        sideDist.x = (this->r_mapPos.x + 1.0f - (player.getPosition().x / world.mapSize.x)) * deltaDist.x;
    }
    if (this->r_rayDir.y < 0.f)
    {
        this->r_stepSize.y = -1;
        sideDist.y = ((player.getPosition().y / world.mapSize.y) - this->r_mapPos.y) * deltaDist.y;
    }
    else
    {
        this->r_stepSize.y = 1;
        sideDist.y = (this->r_mapPos.y + 1.0f - (player.getPosition().y / world.mapSize.y)) * deltaDist.y;
    }

    float fDistance = 0.f;
    while (!this->hit  && fDistance < 1000.f)
    {
        if (sideDist.x < sideDist.y)
        {
            fDistance = sideDist.x;
            sideDist.x += deltaDist.x;
            this->r_mapPos.x += this->r_stepSize.x;
            this->side = false;
        }
        else
        {
            fDistance = sideDist.y;
            sideDist.y += deltaDist.y;
            this->r_mapPos.y += this->r_stepSize.y;
            this->side = true;
        }
        if (this->r_mapPos.x >= 0 && this->r_mapPos.x <= world.mapWidth && this->r_mapPos.y >= 0 && this->r_mapPos.y <= world.mapHeight)
            if (world.getMapTile(this->r_mapPos.x, this->r_mapPos.y) > 0)
                this->hit = true;
    }
    
    if (fDistance > 10.f)
        fDistance = 10.f;

    int a = i + 1;
    this->r_vertices[a].position = sf::Vector2f(player.getPosition().x + this->r_rayDir.x * fDistance * world.mapSize.x,
                                                player.getPosition().y + this->r_rayDir.y * fDistance * world.mapSize.y);

    if (!this->side) this->perpWallDist = (sideDist.x - deltaDist.x);
    else             this->perpWallDist = (sideDist.y - deltaDist.y);

    //TODO: aspect ratio??? or something idk... fix later...
    int lineHeight = (int)(screenHeight / this->perpWallDist);
    sf::Vector2i draw;
    draw.x = (int)(-lineHeight / 2.f + screenHeight / 2.f * player.m_angle);
    draw.y = (int)(lineHeight / 2.f + screenHeight / 2.f * player.m_angle);

    sf::Vertex* line = &this->r_walls[static_cast<std::size_t>(i) * 2];
    line[0].position = sf::Vector2f((float)i, (float)draw.x);
    line[1].position = sf::Vector2f((float)i, (float)draw.y);

    sf::Color color;
    color = sf::Color(
        255 - (int)(this->perpWallDist * 20 > 255 ? 255 : this->perpWallDist * 20),
        255 - (int)(this->perpWallDist * 20 > 255 ? 255 : this->perpWallDist * 20),
        255 - (int)(this->perpWallDist * 20 > 255 ? 255 : this->perpWallDist * 20), 255);

    if (this->side)
    {
        color.r = sf::Uint8(color.r / 2.5f);
        color.g = sf::Uint8(color.g / 2.5f);
        color.b = sf::Uint8(color.b / 2.5f);
    }

    line[0].color = color;
    line[1].color = color;

    float wallX;
    if (!this->side) wallX = player.getPosition().y / world.mapSize.y + this->perpWallDist * this->r_rayDir.y;
    else             wallX = player.getPosition().x / world.mapSize.x + this->perpWallDist * this->r_rayDir.x;
    wallX -= floor((wallX));

    int texX = int(wallX * texWidth);
    if (!this->side && this->r_rayDir.x > 0.f) texX = texWidth - texX - 1;
    if (this->side && this->r_rayDir.y < 0.f) texX = texWidth - texX - 1;

    int mapNum = world.getMapTile(this->r_mapPos.x, this->r_mapPos.y);
    line[0].texCoords = sf::Vector2f(texX + (texWidth * mapNum + 0.5f), 0.f);
    line[1].texCoords = sf::Vector2f(texX + (texWidth * mapNum + 0.5f), (float)texHeight);
}

sf::Vertex& ray::operator[](const std::size_t& index)
{
    if (index > this->r_vertices.getVertexCount())
        throw "Out of bound...\n";
    return this->r_vertices[index];
}

void ray::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(this->r_walls, states);
    target.draw(this->r_vertices);
}