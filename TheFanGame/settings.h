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
	settings() {};
	virtual ~settings() {};

	static const bool saveSettings(const std::string& filePath) noexcept;
	static const bool loadSettings(const std::string& filePath) noexcept;

	static std::string m_currMusic;
	static std::array<const char*, 2> m_Music;
	static std::vector<sf::VideoMode> m_Videomodes;
	static int m_currVideomode;
	static bool m_Fullscreen;
	static bool m_ShowFPS;
	static bool m_isFPSLimited;
	static bool m_Vsync;
	static int m_FpsLimit;
	static float m_VerticalSensivity;
	static float m_HorizontalSensivity;
	static float m_GameVolume;
	static float m_MusicVolume;
	static int m_currProfilePicture;
	static int m_currFrontPicture;
	static int m_currBackgroundPicture;
public:
};