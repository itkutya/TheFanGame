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
	animation() noexcept;
	virtual ~animation() noexcept;

	const bool update(const sf::Time& speed) noexcept;
private:
	sf::Clock timer;
};

