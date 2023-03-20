#pragma once

#include "Menu.h"
#include "Managers/InputManager.h"

class Application
{
	StateManager& s_StateManager = StateManager::getInstance();
	SettingsManager& s_Settings_Manager = SettingsManager::getInstance();
	InputManager& s_InputManager = InputManager::getInstance();
public:
	explicit Application(const std::string& title) noexcept;
	virtual ~Application() noexcept;

	[[nodiscard]] sf::RenderWindow& getWindow() noexcept;

	void pollEvents() noexcept;
	void update() noexcept;
	void draw() noexcept;

	void recreateWindow();

	std::vector<sf::VideoMode> m_videomodes = sf::VideoMode::getFullscreenModes();
	int& m_videomode = this->s_Settings_Manager.get("videomode").m_int;
	sf::VideoMode m_size = this->m_videomodes[this->m_videomode];
	bool& m_fullscreen = this->s_Settings_Manager.get("fullscreen").m_bool;
	int& m_fpslimit = this->s_Settings_Manager.get("fpslimit").m_int;
	bool& m_fps = this->s_Settings_Manager.get("fps").m_bool;
	bool& m_showfps = this->s_Settings_Manager.get("showfps").m_bool;
	bool& m_vsync = this->s_Settings_Manager.get("vsync").m_bool;
private:
	sf::RenderWindow m_window;
	sf::Clock m_deltatime;
	const std::string m_title;

	static constexpr ImGuiWindowFlags s_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | 
												ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs | 
												ImGuiWindowFlags_NoMove;
};