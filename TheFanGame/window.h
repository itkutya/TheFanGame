#pragma once

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Graphics.hpp"
	#else
		#include "SFML32/Graphics.hpp"
	#endif
#endif

class window
{
public:
	window(const sf::VideoMode& size, const char* name);
	virtual ~window();

	operator bool() const;

	const void pollEvents();
private:
	sf::RenderWindow m_window;
	const sf::VideoMode videoMode;
	const char* title;

	sf::Event m_event;
};

