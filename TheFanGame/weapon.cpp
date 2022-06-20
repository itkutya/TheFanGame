#include "weapon.h"

weapon::weapon(const float& dmg, const int& maxAmmo, const sf::Time& dps, const float& range, const bool& isMelee) noexcept 
                : w_dmg(dmg), w_maxAmmoCap(maxAmmo), w_currAmmo(maxAmmo), w_DPS(dps), w_range(range), w_isMelee(isMelee)
{
    this->w_clock.restart();
    this->hitPos.reserve(maxAmmo);
    this->distPos.reserve(maxAmmo);
    this->w_angle.reserve(maxAmmo);
    this->w_impactPoint.reserve(maxAmmo);
}

weapon::~weapon() noexcept {}

const void weapon::shoot(entity& entity, world& world, const sf::Vector2u& screenSize) noexcept
{
    if (this->w_currAmmo > 0 && this->w_clock.getElapsedTime().asSeconds() > this->w_DPS.asSeconds())
    {
        bool hit = false;
        bool side = false;
        sf::Vector2f rayDir = sf::Vector2f();
        sf::Vector2i mapPos = sf::Vector2i();

        rayDir.x = entity.m_direction.x;
        rayDir.y = entity.m_direction.y;

        mapPos.x = (int)(entity.getPosition().x / world.mapSize.x);
        mapPos.y = (int)(entity.getPosition().y / world.mapSize.y);

        sf::Vector2f deltaDist = sf::Vector2f();
        deltaDist.x = (rayDir.x == 0.f) ? 1e30f : std::abs(1.f / rayDir.x);
        deltaDist.y = (rayDir.y == 0.f) ? 1e30f : std::abs(1.f / rayDir.y);

        sf::Vector2f sideDist = sf::Vector2f();
        sf::Vector2i stepSize = sf::Vector2i();
        if (rayDir.x < 0.f)
        {
            stepSize.x = -1;
            sideDist.x = ((entity.getPosition().x / world.mapSize.x) - mapPos.x) * deltaDist.x;
        }
        else
        {
            stepSize.x = 1;
            sideDist.x = (mapPos.x + 1.0f - (entity.getPosition().x / world.mapSize.x)) * deltaDist.x;
        }
        if (rayDir.y < 0.f)
        {
            stepSize.y = -1;
            sideDist.y = ((entity.getPosition().y / world.mapSize.y) - mapPos.y) * deltaDist.y;
        }
        else
        {
            stepSize.y = 1;
            sideDist.y = (mapPos.y + 1.0f - (entity.getPosition().y / world.mapSize.y)) * deltaDist.y;
        }

        float fDistance = 0.f;
        while (!hit && fDistance < this->w_range)
        {
            if (sideDist.x < sideDist.y)
            {
                fDistance = sideDist.x;
                sideDist.x += deltaDist.x;
                mapPos.x += stepSize.x;
                side = false;
            }
            else
            {
                fDistance = sideDist.y;
                sideDist.y += deltaDist.y;
                mapPos.y += stepSize.y;
                side = true;
            }
            if (mapPos.x >= 0 && mapPos.x <= world.mapWidth && mapPos.y >= 0 && mapPos.y <= world.mapHeight)
                if (world.getMapTile(mapPos.x, mapPos.y) > 0)
                    hit = true;
        }

        if (this->hitPos.size() == this->w_maxAmmoCap)
            this->hitPos.clear();
        this->hitPos.emplace_back(sf::Vector2f(entity.getPosition().x + rayDir.x * fDistance * world.mapSize.x, entity.getPosition().y + rayDir.y * fDistance * world.mapSize.y));
        
        if (this->distPos.size() == this->w_maxAmmoCap)
            this->distPos.clear();
        this->distPos.emplace_back(sf::Vector2f((entity.getPosition().x + rayDir.x * fDistance * world.mapSize.x) - entity.getPosition().x, (entity.getPosition().y + rayDir.y * fDistance * world.mapSize.y) - entity.getPosition().y));

        if (this->w_angle.size() == this->w_maxAmmoCap)
            this->w_angle.clear();
        this->w_angle.emplace_back(entity.m_angle);
        
        if (this->w_impactPoint.size() == this->w_maxAmmoCap)
            this->w_impactPoint.clear();
        this->w_impactPoint.emplace_back(std::make_unique<quad>(sf::Vector2f(5.f, 5.f), sf::Vector2f(0.f, 0.f)));

        this->w_clock.restart();
        this->w_currAmmo--;
        std::cout << w_currAmmo << "\\" << w_maxAmmoCap << '\n';
    }
}

const void weapon::reload() noexcept 
{
    this->hitPos.clear();
    this->distPos.clear();
    this->w_angle.clear();
    this->w_impactPoint.clear();
    this->w_currAmmo = this->w_maxAmmoCap; 
}

const void weapon::update(entity& entity, const sf::Vector2u& screenSize) noexcept
{
    for (std::size_t i = 0; i < this->hitPos.size(); i++)
    {
        float spriteX = this->hitPos[i].x - entity.getPosition().x;
        float spriteY = this->hitPos[i].y - entity.getPosition().y;
        float invDet = 1.f / (entity.m_plane.x * entity.m_direction.y - entity.m_direction.x * entity.m_plane.y);
        float transformX = invDet * (entity.m_direction.y * spriteX - entity.m_direction.x * spriteY);
        float transformY = invDet * (-entity.m_plane.y * spriteX + entity.m_plane.x * spriteY);

        float distance = std::hypotf(spriteX, spriteY) / std::hypotf(this->distPos[i].x, this->distPos[i].y);
        int spriteHeight = abs((int)((screenSize.y / transformY)));
        int drawStartY = (int)(-spriteHeight / 2.f + screenSize.y / 2.f - entity.m_angle + this->w_angle[i] / distance);
        int drawEndY = (int)(spriteHeight / 2.f + screenSize.y / 2.f - entity.m_angle + this->w_angle[i] / distance);

        int spriteScreenX = (int)((screenSize.x / 2.f) * (1.f + (transformX / transformY)));
        int spriteWidth = abs((int)((screenSize.y / transformY)));
        int drawStartX = (int)(-spriteWidth / 2.f + spriteScreenX);
        int drawEndX = (int)(spriteWidth / 2.f + spriteScreenX);

        if (transformY > 0.f && this->w_impactPoint[i] != nullptr)
            this->w_impactPoint[i]->setPosition((drawStartX + drawEndX) / 2.f, (drawStartY + drawEndY) / 2.f);
    }
}

void weapon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (auto& i : this->w_impactPoint)
        if (i != nullptr)
            target.draw(*i, states);
}
