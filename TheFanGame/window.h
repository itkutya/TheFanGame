#pragma once

#include <ctime>
#include <iomanip>
#include <sstream>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui-SFML.h"

#include "ImGUI/imgui_notify.h"
#include "ImGUI/tahoma.h"

#include "stateSystem.h"
#include "resourceSystem.h"
#include "inputSystem.h"
#include "settings.h"

class window
{
public:
	//Construct's the window class.
	window(const char* name) noexcept;
	//Destruct's the window class.
	virtual ~window() noexcept;
	//Returns m_window.isOpen().
	explicit operator const bool() noexcept;
	//Returns the address of m_window.
	operator sf::RenderWindow&() noexcept;
	//Set FPS limit.
	const void setFramerateLimit() noexcept;
	//Set VSync to true.
	const void setVSync() noexcept;
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
	const char* title;
	//Takes a screenshot in any given state.
	const void ScreenShot() noexcept;
};