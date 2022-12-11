#pragma once

#include "stateSystem.h"
#include "resourceSystem.h"
#include "inputSystem.h"
#include "settings.h"
#include "gui.h"

class window
{
public:
	window(const sf::VideoMode& vm, const std::string& name, const bool& fs, const std::uint32_t& limit, const bool& vsync) noexcept;
	virtual ~window() noexcept = default;

	const void setFramerateLimit(const std::uint32_t& limit) noexcept;
	const void setVSync(const bool& vsync) noexcept;
	const void create(const sf::VideoMode& vm, const std::string& name, const bool& fs, const std::uint32_t& limit, const bool& vsync) noexcept;

	const void pollEvents() noexcept;
	const void draw() noexcept;
	const void update() noexcept;

	const sf::View& getView() noexcept;
	sf::RenderWindow& getWindow() noexcept;
private:
	sf::RenderWindow m_window;
	sf::Clock deltaTime;
	sf::View m_view;

	const void ScreenShot() noexcept;
};