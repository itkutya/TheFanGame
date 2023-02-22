#pragma once

#include "Menu.h"

class Application
{
public:
	explicit Application(const sf::VideoMode& size, const std::string& title, const bool& fullscreen, const std::uint32_t& fpslimit = 0) noexcept;
	virtual ~Application() noexcept;

	[[nodiscard]] sf::RenderWindow& getWindow() noexcept;

	void pollEvents() noexcept;
	void update() noexcept;
	void draw() noexcept;
private:
	sf::RenderWindow m_window;
	sf::View m_view;
	sf::Clock m_deltatime;

	StateManager* s_StateManager;
};