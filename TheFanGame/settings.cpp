#include "settings.h"

std::array<const char*, 2> settings::m_Music = { {"Blackbird - Cecile Corbel", "Sakakibara Yui - Koi no Honoo"} };
std::vector<sf::VideoMode> settings::m_Videomodes = sf::VideoMode::getFullscreenModes();
std::string settings::m_currMusic = "";
int settings::m_currVideomode = 0;
bool settings::m_Fullscreen = false;
int settings::m_FpsLimit = 60;
bool settings::m_ShowFPS = true;
bool settings::m_isFPSLimited = true;
bool settings::m_Vsync = false;
float settings::m_VerticalSensivity = 3.0f;
float settings::m_HorizontalSensivity = 3.0f;
float settings::m_GameVolume = 100.f;
float settings::m_MusicVolume = 100.f;
int settings::m_currProfilePicture = 0;
int settings::m_currFrontPicture = 0;
int settings::m_currBackgroundPicture = 0;

const bool settings::saveSettings(const std::string& filePath) noexcept
{
	std::ofstream saveFile(filePath, std::ios::out | std::ios::trunc);
	if (saveFile.is_open())
	{
		saveFile << settings::m_currMusic << '\n' << settings::m_currVideomode << '\n' << settings::m_Fullscreen << '\n' 
			<< settings::m_FpsLimit << '\n' << settings::m_ShowFPS << '\n' << settings::m_isFPSLimited << '\n'
			<< settings::m_Vsync << '\n' << settings::m_VerticalSensivity << '\n' << settings::m_HorizontalSensivity << '\n'
			<< settings::m_GameVolume << '\n' << settings::m_MusicVolume << '\n' << settings::m_currProfilePicture << '\n'
			<< settings::m_currFrontPicture << '\n' << settings::m_currBackgroundPicture << '\n';
	}
	else
	{
		saveFile.close();
		return false;
	}
	saveFile.close();
	return true;
}

const bool settings::loadSettings(const std::string& filePath) noexcept
{
	std::ifstream loadFile(filePath);
	if (loadFile.is_open())
	{
		std::getline(loadFile, settings::m_currMusic);
		loadFile >> settings::m_currVideomode >> settings::m_Fullscreen >> settings::m_FpsLimit >>
			settings::m_ShowFPS >> settings::m_isFPSLimited >> settings::m_Vsync >> settings::m_VerticalSensivity >> 
			settings::m_HorizontalSensivity >> settings::m_GameVolume >> settings::m_MusicVolume >> settings::m_currProfilePicture >> 
			settings::m_currFrontPicture >> settings::m_currBackgroundPicture;
	}
	else
	{
		loadFile.close();
		return false;
	}
	loadFile.close();
	return true;
}