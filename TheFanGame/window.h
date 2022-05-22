#pragma once

#include <iostream>
#include <vector>

#include "entity.h"

class window
{
public:
	window(const sf::VideoMode& size, const char* name);
	virtual ~window();

	operator const bool();

	const void setFramerateLimit(const unsigned int& limit);
	void addToDrawPool(const sf::Drawable* object);

	const void draw();
	void update();
private:
	sf::RenderWindow m_window;
	sf::Event m_event;
	sf::Clock deltaTime;

	std::vector<const sf::Drawable*> objects;

	entity quad = entity(sf::PrimitiveType::Quads, 4);
	
	const void pollEvents();
};