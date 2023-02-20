#include "settings.h"

settings::~settings() noexcept { saveSettings(m_filePath); }

const bool settings::loadSettings(const char* filePath) noexcept
{
	std::ifstream loadFile(filePath);
	if (loadFile.is_open())
	{
		std::getline(loadFile, m_currMusic);
		loadFile >> m_currVideomode >> m_Fullscreen >> m_FpsLimit >>
			m_ShowFPS >> m_isFPSLimited >> m_Vsync >> m_VerticalSensivity >>
			m_HorizontalSensivity >> m_GameVolume >> m_MusicVolume >> m_currProfilePicture >>
			m_currFrontPicture >> m_currBackgroundPicture >> ProfileNickname >> ProfilePassword >> 
			logged_in >> rememberToStayLogedIn;
	}
	else
	{
		loadFile.close();
		return false;
	}
	loadFile.close();
	return true;
}

const bool settings::saveSettings(const char* filePath) noexcept
{
	std::ofstream saveFile(filePath, std::ios::out | std::ios::trunc);
	if (saveFile.is_open())
	{
		saveFile << m_currMusic << '\n' << m_currVideomode << '\n' << m_Fullscreen << '\n' 
			<< m_FpsLimit << '\n' << m_ShowFPS << '\n' << m_isFPSLimited << '\n'
			<< m_Vsync << '\n' << m_VerticalSensivity << '\n' << m_HorizontalSensivity << '\n'
			<< m_GameVolume << '\n' << m_MusicVolume << '\n' << m_currProfilePicture << '\n'
			<< m_currFrontPicture << '\n' << m_currBackgroundPicture << '\n' << ProfileNickname << '\n'
			<< ProfilePassword << '\n' << logged_in << '\n' << rememberToStayLogedIn << '\n';
	}
	else
	{
		saveFile.close();
		return false;
	}
	saveFile.close();
	return true;
}