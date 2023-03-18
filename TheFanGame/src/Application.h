#pragma once

#include "Menu.h"

class Application
{
	StateManager& s_StateManager = StateManager::getInstance();
	SettingsManager& s_Settings_Manager = SettingsManager::getInstance();
public:
	explicit Application(const std::string& title) noexcept;
	virtual ~Application() noexcept;

	[[nodiscard]] sf::RenderWindow& getWindow() noexcept;

	void pollEvents() noexcept;
	void update() noexcept;
	void draw() noexcept;

	void recreateWindow();

	std::vector<sf::VideoMode> m_videomodes = sf::VideoMode::getFullscreenModes();
	int& m_videomode = this->s_Settings_Manager.get<int>("videomode");
	sf::VideoMode m_size = this->m_videomodes[this->m_videomode];
	bool& m_fullscreen = this->s_Settings_Manager.get<bool>("fullscreen");
	int& m_fpslimit = this->s_Settings_Manager.get<int>("fpslimit");
	bool& m_fps = this->s_Settings_Manager.get<bool>("fps");
	bool& m_showfps = this->s_Settings_Manager.get<bool>("showfps");
	bool& m_vsync = this->s_Settings_Manager.get<bool>("vsync");
private:
	sf::RenderWindow m_window;
	sf::Clock m_deltatime;
	const std::string m_title;
};