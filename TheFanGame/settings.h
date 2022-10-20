#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <array>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Graphics.hpp"
	#else
		#include "SFML32/Graphics.hpp"
	#endif
#endif

#include "resourceSystem.h"

class settings
{
public:
	explicit settings(const char* filePath) noexcept;
	settings(const settings&) = delete;
	settings(const settings&&) = delete;
	settings& operator=(settings& other) = delete;
	settings& operator=(const settings& other) = delete;
	virtual ~settings() noexcept;

	const bool saveSettings(const char* filePath) noexcept;
	
	std::array<const char*, 2> m_Music = { {"Blackbird - Cecile Corbel", "Sakakibara Yui - Koi no Honoo"} };
	std::vector<sf::VideoMode> m_Videomodes = sf::VideoMode::getFullscreenModes();
	std::string m_currMusic = "";
	int m_currVideomode = 0;
	bool m_Fullscreen = false;
	int m_FpsLimit = 60;
	bool m_ShowFPS = true;
	bool m_isFPSLimited = true;
	bool m_Vsync = false;
	float m_VerticalSensivity = 3.0f;
	float m_HorizontalSensivity = 3.0f;
	float m_GameVolume = 100.f;
	float m_MusicVolume = 100.f;
	int m_currProfilePicture = 0;
	int m_currFrontPicture = 0;
	int m_currBackgroundPicture = 0;
private:
	const char* m_filePath;
};