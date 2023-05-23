#pragma once

#include "Menu.h"
#include "Managers/Input/InputManager.h"

class Application
{
	SettingsManager& s_SettingsManager = SettingsManager::getInstance();
	StateManager& s_StateManager = StateManager::getInstance();
	InputManager& s_InputManager = InputManager::getInstance();
	AudioManager& s_AudioManager = AudioManager::getInstance();
public:
	explicit Application(const std::string& title) noexcept;
	virtual ~Application() noexcept;

	[[nodiscard]] sf::RenderWindow& getWindow() noexcept;

	void pollEvents() noexcept;
	void update() noexcept;
	void draw() noexcept;

	void recreateWindow();

	std::vector<sf::VideoMode> m_videomodes = sf::VideoMode::getFullscreenModes();
	int& m_videomode = this->s_SettingsManager["Setting"]["VideoMode"];
	sf::VideoMode m_size = this->m_videomodes[this->m_videomode];
	bool& m_fullscreen = this->s_SettingsManager["Setting"]["Fullscreen"];
	int& m_fpslimit = this->s_SettingsManager["Setting"]["FPSLimit"];
	bool& m_fps = this->s_SettingsManager["Setting"]["FPS"];
	bool& m_showfps = this->s_SettingsManager["Setting"]["ShowFPS"];
	bool& m_vsync = this->s_SettingsManager["Setting"]["Vsync"];
private:
	sf::RenderWindow m_window;
	sf::Clock m_deltatime;
	const std::string m_title;
};