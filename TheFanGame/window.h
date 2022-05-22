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

	inline const void setFramerateLimit(const unsigned int& limit) { this->m_window.setFramerateLimit(limit); };

	const void draw();
	void update();
private:
	sf::RenderWindow m_window;
	sf::Event m_event;
	sf::Clock deltaTime;

	std::vector<const sf::Drawable*> objects;

	const void pollEvents();
	inline const void addToDrawPool(const sf::Drawable* object) { this->objects.emplace_back(object); };

	entity quad = entity(sf::PrimitiveType::Quads, 4);
};