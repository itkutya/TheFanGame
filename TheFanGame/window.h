#pragma once

#include "entity.h"
#include "animation.h"
#include "gui.h"

class window
{
public:
	window(const sf::VideoMode& size, const char* name) noexcept;
	virtual ~window() noexcept;

	explicit operator const bool() noexcept;

	const void setFramerateLimit(const unsigned int& limit) noexcept;

	const void pollEvents() noexcept;
	const void draw() noexcept;
	const void update() noexcept;
private:
	sf::RenderWindow m_window;
	sf::Clock deltaTime;
	
	std::unique_ptr<stateSystem> m_states;

	std::vector<const sf::Drawable*> objects;
	const void addToDrawPool(const sf::Drawable* object) noexcept;

};