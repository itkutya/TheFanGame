#pragma once

#include <ctime>
#include <iomanip>
#include <sstream>

#include "stateSystem.h"
#include "resourceManager.h"
#include "inputManager.h"

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
	//Add's a state to the state pool.
	template<typename T>
	inline const void addState(const bool& replace = false) { this->m_states.add(this->m_window, std::make_unique<T>(*this), replace); }
	//Set's the framerate of the m_window.
	const void setFramerateLimit(const std::uint32_t& limit) noexcept;
	//Processes the state changes.
	const void processStateChange(sf::RenderWindow& window) noexcept;
	//Delete current state.
	const void popCurrent();
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
	stateSystem m_states;

	sf::Clock deltaTime;

	sf::VideoMode m_videomode;
	std::string title;
	bool isFullscreen;
	int FPSLimit;

	//On the m_window resize this function will triger.
	const void onResize() noexcept;
	//Takes a screenshot in any given state.
	const void ScreenShot() noexcept;
};