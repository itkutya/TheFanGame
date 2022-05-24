#pragma once

#include <iostream>
#include <vector>

#include "entity.h"
#include "animation.h"
#include "gui.h"

class window
{
public:
	window(const sf::VideoMode& size, const char* name) noexcept;
	virtual ~window() noexcept;

	explicit operator const bool();

	const void setFramerateLimit(const unsigned int& limit);

	const void pollEvents ();
	const void draw();
	const void update();
private:
	sf::RenderWindow m_window;
	sf::Clock deltaTime;

	std::vector<const sf::Drawable*> objects;
	const void addToDrawPool(const sf::Drawable* object);

	entity quad = entity(sf::PrimitiveType::Quads, 4, "res/wolftextures.png");
	animation anim;
	bool e = false;
	float r = 12.f;
	sf::CircleShape c = sf::CircleShape(r);
	gui MENU;
};