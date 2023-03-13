#pragma once

#include "Menu.h"

class Application
{
public:
	explicit Application(sf::VideoMode size, const std::string& title, bool fullscreen, std::uint32_t fpslimit = 0) noexcept;
	virtual ~Application() noexcept;

	[[nodiscard]] sf::RenderWindow& getWindow() noexcept;

	void pollEvents() noexcept;
	void update() noexcept;
	void draw() noexcept;

	void recreateWindow();

	sf::VideoMode m_size;
	std::string m_title;
	bool m_fullscreen = false;
	std::uint32_t m_fpslimit = 0;
	bool m_fps = true;
	bool m_vsync = false;
	std::vector<sf::VideoMode> m_videomodes;
	int m_videomode = 0;
private:
	sf::RenderWindow m_window;
	sf::Clock m_deltatime;

	StateManager& s_StateManager = StateManager::getInstance();
};