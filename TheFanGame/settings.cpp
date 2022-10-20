#include "settings.h"

settings::settings(const char* filePath) noexcept : m_filePath(filePath)
{
	std::ifstream loadFile(filePath);
	if (loadFile.is_open())
	{
		std::getline(loadFile, this->m_currMusic);
		loadFile >> this->m_currVideomode >> this->m_Fullscreen >> this->m_FpsLimit >>
			this->m_ShowFPS >> this->m_isFPSLimited >> this->m_Vsync >> this->m_VerticalSensivity >>
			this->m_HorizontalSensivity >> this->m_GameVolume >> this->m_MusicVolume >> this->m_currProfilePicture >>
			this->m_currFrontPicture >> this->m_currBackgroundPicture;
	}
	loadFile.close();
}

settings::~settings() noexcept { this->saveSettings(this->m_filePath); }

const bool settings::saveSettings(const char* filePath) noexcept
{
	std::ofstream saveFile(filePath, std::ios::out | std::ios::trunc);
	if (saveFile.is_open())
	{
		saveFile << this->m_currMusic << '\n' << this->m_currVideomode << '\n' << this->m_Fullscreen << '\n' 
			<< this->m_FpsLimit << '\n' << this->m_ShowFPS << '\n' << this->m_isFPSLimited << '\n'
			<< this->m_Vsync << '\n' << this->m_VerticalSensivity << '\n' << this->m_HorizontalSensivity << '\n'
			<< this->m_GameVolume << '\n' << this->m_MusicVolume << '\n' << this->m_currProfilePicture << '\n'
			<< this->m_currFrontPicture << '\n' << this->m_currBackgroundPicture << '\n';
	}
	else
	{
		saveFile.close();
		return false;
	}
	saveFile.close();
	return true;
}