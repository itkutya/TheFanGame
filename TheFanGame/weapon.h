#pragma once

#include "world.h"
#include "entity.h"

class weapon : public sf::Drawable, public sf::Transformable
{
public:
	weapon(const float& dmg, const int& maxAmmo, const sf::Time& dps, const float& range, const float& reloadSpeed, const bool& isMelee) noexcept;
	virtual ~weapon() noexcept;

	virtual const void shoot(entity& ent, world& world, std::vector<entity>& entities, const sf::Vector2u& screenSize) noexcept;
	virtual const void reload() noexcept;
	virtual const void update(entity& ent, const sf::Vector2u& screenSize) noexcept;

	int w_maxAmmoCap;
	int w_currAmmo;
	std::vector<std::unique_ptr<entity>> w_impactPoint;
	std::vector<sf::Vector2f> hitPos;
	std::vector<sf::Vector2f> distPos;
	std::vector<float> w_angle;
	float w_dmg;
	float w_range;
	float w_reloadSpeed;
	bool w_isMelee;

	sf::Clock w_clock;
	sf::Clock w_reload;
	sf::Time w_DPS;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

