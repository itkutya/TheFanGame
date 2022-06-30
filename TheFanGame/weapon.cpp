#include "weapon.h"

weapon::weapon(const float& dmg, const int& maxAmmo, const sf::Time& dps, const float& range, const float& reloadSpeed, const bool& isMelee) noexcept
                : w_dmg(dmg), w_maxAmmoCap(maxAmmo), w_currAmmo(maxAmmo), w_DPS(dps), w_range(range), w_reloadSpeed(reloadSpeed), w_isMelee(isMelee)
{
    this->w_clock.restart();
    this->w_reload.restart();
    this->hitPos.reserve(maxAmmo);
    this->distPos.reserve(maxAmmo);
    this->w_angle.reserve(maxAmmo);
    this->w_impactPoint.reserve(maxAmmo);
}

weapon::~weapon() noexcept {}

const void weapon::shoot(entity& ent, world& world, std::vector<entity>& entities, const sf::Vector2u& screenSize) noexcept
{
    if (this->w_currAmmo > 0 && this->w_clock.getElapsedTime().asSeconds() > this->w_DPS.asSeconds() && !this->w_isMelee && this->w_reload.getElapsedTime().asSeconds() > this->w_reloadSpeed)
    {
        bool hit = false;
        bool side = false;
        sf::Vector2f rayDir = sf::Vector2f();
        sf::Vector2i mapPos = sf::Vector2i();

        rayDir.x = ent.m_direction.x;
        rayDir.y = ent.m_direction.y;

        mapPos.x = (int)(ent.getPosition().x / world.mapSize.x);
        mapPos.y = (int)(ent.getPosition().y / world.mapSize.y);

        sf::Vector2f deltaDist = sf::Vector2f();
        deltaDist.x = (rayDir.x == 0.f) ? 1e30f : std::abs(1.f / rayDir.x);
        deltaDist.y = (rayDir.y == 0.f) ? 1e30f : std::abs(1.f / rayDir.y);

        sf::Vector2f sideDist = sf::Vector2f();
        sf::Vector2i stepSize = sf::Vector2i();
        if (rayDir.x < 0.f)
        {
            stepSize.x = -1;
            sideDist.x = ((ent.getPosition().x / world.mapSize.x) - mapPos.x) * deltaDist.x;
        }
        else
        {
            stepSize.x = 1;
            sideDist.x = (mapPos.x + 1.0f - (ent.getPosition().x / world.mapSize.x)) * deltaDist.x;
        }
        if (rayDir.y < 0.f)
        {
            stepSize.y = -1;
            sideDist.y = ((ent.getPosition().y / world.mapSize.y) - mapPos.y) * deltaDist.y;
        }
        else
        {
            stepSize.y = 1;
            sideDist.y = (mapPos.y + 1.0f - (ent.getPosition().y / world.mapSize.y)) * deltaDist.y;
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
            for (std::size_t i = 0; i < entities.size(); ++i)
            {
                if (entities[i].m_sprites[0].position.x < screenSize.x / 2.f &&
                    entities[i].m_sprites[1].position.x > screenSize.x / 2.f &&
                    entities[i].m_sprites[0].position.y < screenSize.y / 2.f &&
                    entities[i].m_sprites[3].position.y > screenSize.y / 2.f)
                {
                    if (fDistance >= std::hypotf(ent.getPosition().x - entities[i].getPosition().x, ent.getPosition().y - entities[i].getPosition().y) / ((world.mapSize.x + world.mapSize.y) / 2.f))
                    {
                        fDistance = std::hypotf(ent.getPosition().x - entities[i].getPosition().x, ent.getPosition().y - entities[i].getPosition().y) / ((world.mapSize.x + world.mapSize.y) / 2.f);
                        entities[i].health -= this->w_dmg;
                        hit = true;
                    }
                }
            }
        }

        if (hit)
        {
            this->hitPos.emplace_back(sf::Vector2f(ent.getPosition().x + rayDir.x * fDistance * world.mapSize.x, ent.getPosition().y + rayDir.y * fDistance * world.mapSize.y));
            this->distPos.emplace_back(sf::Vector2f((ent.getPosition().x + rayDir.x * fDistance * world.mapSize.x) - ent.getPosition().x, (ent.getPosition().y + rayDir.y * fDistance * world.mapSize.y) - ent.getPosition().y));
            this->w_angle.emplace_back(ent.m_angle);
            this->w_impactPoint.emplace_back(std::make_unique<entity>(sf::Vector2f(5.f, 5.f), sf::Vector2f(0.f, 0.f), sf::Color::Yellow));
        }
        else
        {
            this->hitPos.emplace_back(sf::Vector2f(ent.getPosition().x + rayDir.x * fDistance * world.mapSize.x, ent.getPosition().y + rayDir.y * fDistance * world.mapSize.y));
            this->distPos.emplace_back(sf::Vector2f((ent.getPosition().x + rayDir.x * fDistance * world.mapSize.x) - ent.getPosition().x, (ent.getPosition().y + rayDir.y * fDistance * world.mapSize.y) - ent.getPosition().y));
            this->w_angle.emplace_back(ent.m_angle);
            this->w_impactPoint.emplace_back(std::make_unique<entity>(sf::Vector2f(5.f, 5.f), sf::Vector2f(0.f, 0.f), sf::Color::Red));
        }

        this->w_clock.restart();
        this->w_currAmmo--;
    }
}

const void weapon::reload() noexcept
{
    if (this->w_reload.getElapsedTime().asSeconds() > this->w_reloadSpeed && this->w_currAmmo != this->w_maxAmmoCap)
    {
        this->hitPos.clear();
        this->distPos.clear();
        this->w_angle.clear();
        this->w_impactPoint.clear();
        this->w_currAmmo = this->w_maxAmmoCap;

        this->w_reload.restart();
    }
}

const void weapon::update(entity& ent, const sf::Vector2u& screenSize) noexcept
{
    for (std::size_t i = 0; i < this->hitPos.size(); i++)
    {
        float spriteX = this->hitPos[i].x - ent.getPosition().x;
        float spriteY = this->hitPos[i].y - ent.getPosition().y;
        float invDet = 1.f / (ent.m_plane.x * ent.m_direction.y - ent.m_direction.x * ent.m_plane.y);
        float transformX = invDet * (ent.m_direction.y * spriteX - ent.m_direction.x * spriteY);
        float transformY = invDet * (-ent.m_plane.y * spriteX + ent.m_plane.x * spriteY);

        float distance = std::hypotf(spriteX, spriteY) / std::hypotf(this->distPos[i].x, this->distPos[i].y);

        int spriteHeight = abs((int)((screenSize.y / transformY)));
        int drawStartY = (int)(-spriteHeight / 2.f + screenSize.y / 2.f - ent.m_angle + this->w_angle[i] / distance);
        int drawEndY = (int)(spriteHeight / 2.f + screenSize.y / 2.f - ent.m_angle + this->w_angle[i] / distance);

        int spriteScreenX = (int)((screenSize.x / 2.f) * (1.f + (transformX / transformY)));
        int spriteWidth = abs((int)((screenSize.y / transformY)));
        int drawStartX = (int)(-spriteWidth / 2.f + spriteScreenX);
        int drawEndX = (int)(spriteWidth / 2.f + spriteScreenX);

        if (transformY > 0.f)
            this->w_impactPoint[i]->setPosition((drawStartX + drawEndX) / 2.f, (drawStartY + drawEndY) / 2.f);

        this->w_impactPoint[i]->m_vertices[0].color = this->w_impactPoint[i]->getDistanceColor(sf::Vector2f(spriteX, spriteY));
        this->w_impactPoint[i]->m_vertices[1].color = this->w_impactPoint[i]->getDistanceColor(sf::Vector2f(spriteX, spriteY));
        this->w_impactPoint[i]->m_vertices[2].color = this->w_impactPoint[i]->getDistanceColor(sf::Vector2f(spriteX, spriteY));
        this->w_impactPoint[i]->m_vertices[3].color = this->w_impactPoint[i]->getDistanceColor(sf::Vector2f(spriteX, spriteY));
    }
}

void weapon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (auto& i : this->w_impactPoint)
        target.draw(*i, states);
}
