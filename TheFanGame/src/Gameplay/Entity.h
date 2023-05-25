#pragma once

#include "Managers/ResourceManager.h"

class Entity : public sf::Drawable, public sf::Transformable
{
public:
	Entity() = default;
	virtual ~Entity() = default;

	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states) const = 0;

	sf::Vector2f m_position;
	sf::Texture m_texture;
private:
};