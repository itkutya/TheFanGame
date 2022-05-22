#pragma once

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Graphics.hpp"
	#else
		#include "SFML32/Graphics.hpp"
	#endif
#endif

#include <vector>

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

	std::vector<const sf::Drawable*> objects;
	
	const void pollEvents();
};