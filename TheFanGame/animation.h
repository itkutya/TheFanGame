#pragma once

#include <iostream>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/SFML/Graphics.hpp"
	#else
		#include "SFML32/Graphics.hpp"
	#endif
#endif

constexpr int texWidth = 64;
constexpr int texHeight = 64;

class animation
{
public:
	animation() noexcept;
	virtual ~animation() noexcept;

	const bool update(const sf::Time& speed, sf::VertexArray& obj, const std::uint8_t& maxFrame, const sf::FloatRect& pos) noexcept;
private:
	std::uint8_t currentFrame;
	sf::Clock timer;
};

