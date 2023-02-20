#pragma once

#include "includes.h"

class settings
{
public:
	settings(const char* filePath) = delete;
	settings(const settings&) = delete;
	settings(const settings&&) = delete;
	settings& operator=(settings& other) = delete;
	settings& operator=(const settings& other) = delete;
	virtual ~settings() noexcept;

	static const bool loadSettings(const char* filePath) noexcept;
	static const bool saveSettings(const char* filePath) noexcept;
	
	static inline std::array<const char*, 2> m_Music = { {"Blackbird - Cecile Corbel", "Sakakibara Yui - Koi no Honoo"} };
	static inline std::vector<sf::VideoMode> m_Videomodes = sf::VideoMode::getFullscreenModes();
	static inline std::string m_currMusic = std::string("");
	static inline int m_currVideomode = 0;
	static inline bool m_Fullscreen = false;
	static inline int m_FpsLimit = 60;
	static inline bool m_ShowFPS = true;
	static inline bool m_isFPSLimited = true;
	static inline bool m_Vsync = false;
	static inline float m_VerticalSensivity = 3.0f;
	static inline float m_HorizontalSensivity = 3.0f;
	static inline float m_GameVolume = 100.f;
	static inline float m_MusicVolume = 100.f;
	static inline int m_currProfilePicture = 0;
	static inline int m_currFrontPicture = 0;
	static inline int m_currBackgroundPicture = 0;
	static inline std::string ProfileNickname = std::string("");
	static inline std::string ProfilePassword = std::string("");
	static inline bool logged_in = false;
	static inline bool rememberToStayLogedIn = false;
private:
	const char* m_filePath;
};