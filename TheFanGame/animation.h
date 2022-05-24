#pragma once

#include <iostream>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Graphics.hpp"
	#else
		#include "SFML32/Graphics.hpp"
	#endif
#endif

class animation
{
public:
	animation();
	virtual ~animation();

	const bool update(const float& dt, const float& speed);
private:
	sf::Clock timer;
};

