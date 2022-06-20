#pragma once

#include "world.h"
#include "quad.h"

class weapon : public sf::Drawable, public sf::Transformable
{
public:
	weapon(const float& dmg, const int& maxAmmo, const sf::Time& dps, const float& range, const bool& isMelee) noexcept;
	virtual ~weapon() noexcept;

	const void shoot(entity& entity, world& world, const sf::Vector2u& screenSize) noexcept;
	const void reload() noexcept;
	const void update(entity& entity, const sf::Vector2u& screenSize) noexcept;
private:
	std::vector<std::unique_ptr<entity>> w_impactPoint;
	std::vector<sf::Vector2f> hitPos;
	std::vector<sf::Vector2f> distPos;
	std::vector<float> w_angle;
	float w_dmg;
	float w_range;
	bool w_isMelee;
	int w_maxAmmoCap;
	int w_currAmmo;

	sf::Clock w_clock;
	sf::Time w_DPS;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

