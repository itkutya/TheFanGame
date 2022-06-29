#include "ray.h"

ray::ray(const std::uint32_t& screenWidth)
{
	this->r_vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
    this->r_vertices.resize(static_cast<std::size_t>(screenWidth) + 1);

    this->r_walls.setPrimitiveType(sf::PrimitiveType::Lines);
    this->r_walls.resize((static_cast<std::size_t>(screenWidth)) * 2);

    this->r_floors.setPrimitiveType(sf::PrimitiveType::Lines);
    this->r_floors.resize((static_cast<std::size_t>(screenWidth)) * 2);
    this->r_ceilings.setPrimitiveType(sf::PrimitiveType::Lines);
    this->r_ceilings.resize((static_cast<std::size_t>(screenWidth)) * 2);

    this->r_rayDir = sf::Vector2f(0.f, 0.f);
    this->r_stepSize = sf::Vector2i(0, 0);
    this->r_mapPos = sf::Vector2i(0, 0);
    this->side = false;
    this->hit = false;
    this->perpWallDist = 0.f;
}

ray::~ray() {}

const void ray::castRay(player& player, world& world, const sf::Vector2u& screenSize, const std::uint32_t& i)
{
    this->hit = false;

    float cameraX = (float)(2.f * i / screenSize.x - 1.f);
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
    while (!this->hit && fDistance < 1000.f)
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

    int a = i + 1;
    this->r_vertices[a].position = sf::Vector2f(player.getPosition().x + this->r_rayDir.x * fDistance * world.mapSize.x,
        player.getPosition().y + this->r_rayDir.y * fDistance * world.mapSize.y);

    if (!this->side) this->perpWallDist = (sideDist.x - deltaDist.x);
    else             this->perpWallDist = (sideDist.y - deltaDist.y);

    int lineHeight = (int)(screenSize.y / this->perpWallDist);
    sf::Vector2i draw;
    draw.x = (int)(-lineHeight / 2.f + screenSize.y / 2.f - player.m_angle);
    draw.y = (int)(lineHeight / 2.f + screenSize.y / 2.f - player.m_angle);

    sf::Vertex* line = &this->r_walls[std::size_t(i) * 2];
    line[0].position = sf::Vector2f((float)i + 1.f, (float)draw.x);
    line[1].position = sf::Vector2f((float)i + 1.f, (float)draw.y);

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
    wallX -= std::floor((wallX));

    int texX = int(wallX * texWidth);
    if (!this->side && this->r_rayDir.x > 0.f) texX = texWidth - texX - 1;
    if (this->side && this->r_rayDir.y < 0.f)  texX = texWidth - texX - 1;

    int mapNum = world.getMapTile(this->r_mapPos.x, this->r_mapPos.y);
    line[0].texCoords = sf::Vector2f(texX + (texWidth * mapNum + 0.5f), 0.f);
    line[1].texCoords = sf::Vector2f(texX + (texWidth * mapNum + 0.5f), (float)texHeight);

    //floor
    line = &this->r_floors[std::size_t(i) * 2];
    line[0].position = sf::Vector2f((float)i + 1.f, (float)draw.y);
    line[1].position = sf::Vector2f((float)i + 1.f, (float)screenSize.y);
    line[0].color = sf::Color::Black;
    line[1].color = sf::Color(125, 125, 125, 255);

    //ceiling
    line = &this->r_ceilings[std::size_t(i) * 2];
    line[0].position = sf::Vector2f((float)i + 1.f, 0.f);
    line[1].position = sf::Vector2f((float)i + 1.f, (float)draw.x);
    line[0].color = sf::Color::Cyan;
    line[1].color = sf::Color::Cyan;
}

void ray::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(this->r_floors);
    target.draw(this->r_ceilings);
    target.draw(this->r_walls, states.texture);
    target.draw(this->r_vertices, states.transform);
}