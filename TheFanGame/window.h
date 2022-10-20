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
	window(const sf::VideoMode& vm, const std::string& name, const bool& fs, const std::uint32_t& limit, const bool& vsync, engine& e) noexcept;
	virtual ~window() noexcept = default;

	explicit operator const bool() const noexcept;
	operator sf::RenderWindow&() noexcept;

	const void setFramerateLimit(const std::uint32_t& limit) noexcept;
	const void setVSync(const bool& vsync) noexcept;
	const void create(const sf::VideoMode& vm, const std::string& name, const bool& fs, const std::uint32_t& limit, const bool& vsync) noexcept;

	const void pollEvents() noexcept;
	const void draw() noexcept;
	const void update() noexcept;
private:
	sf::RenderWindow m_window;
	sf::Clock deltaTime;
	engine& m_enigne;

	const void ScreenShot() noexcept;
};