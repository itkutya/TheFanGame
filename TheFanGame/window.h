#pragma once

#include <ctime>
#include <iomanip>
#include <sstream>

#include "stateSystem.h"
#include "resourceSystem.h"
#include "inputSystem.h"
#include "setting.h"

class window
{
public:
	//Construct's the window class.
	window() noexcept;
	//Destruct's the window class.
	virtual ~window() noexcept;
	//Returns m_window.isOpen().
	explicit operator const bool() noexcept;
	//Returns the address of m_window.
	operator sf::RenderWindow&() noexcept;
	//Creates the renderwindow with the given size and name.
	const void create(const sf::VideoMode& size, const char* name) noexcept;
	//Set's the framerate of the m_window.
	const void setFramerateLimit(const std::uint32_t& limit) noexcept;
	//Set's the window size.
	const void setSize(const sf::Vector2u& size) noexcept;
	//Set's the window to fullscreen mode.
	const void setFullscreen(const bool& active) noexcept;
	//Recreate's the window.
	const void recreate() noexcept;
	//Poll events in the main loop.
	const void pollEvents() noexcept;
	//Draw's everything on the m_window.
	const void draw() noexcept;
	//Update's the game logic.
	const void update() noexcept;
private:
	sf::RenderWindow m_window;
	sf::Clock deltaTime;

	sf::VideoMode m_videomode;
	std::string title;
	bool isFullscreen;
	int FPSLimit;

	//Takes a screenshot in any given state.
	const void ScreenShot() noexcept;
};