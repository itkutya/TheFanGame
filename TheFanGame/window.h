#pragma once

#include "gui.h"

class window
{
public:
	window(const sf::VideoMode& size, const char* name, context* context) noexcept;
	virtual ~window() noexcept;

	explicit operator const bool() noexcept;
	operator sf::RenderWindow&() noexcept;

	const void setFramerateLimit(const unsigned int& limit) noexcept;

	const void pollEvents() noexcept;
	const void draw() noexcept;
	const void update() noexcept;
private:
	sf::RenderWindow m_window;
	sf::Clock deltaTime;

	context* m_context;

	std::vector<const sf::Drawable*> objects;
	const void addToDrawPool(const sf::Drawable* object) noexcept;
};