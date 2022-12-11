#include "menu.h"

int main()
{
	std::srand((std::uint32_t)std::time(0));
	try
	{
		settings::loadSettings("res/Settings.ini");
		inputSystem is;
		if (!is.loadInput("res/inputSettings.ini"))
			return 1;

		window mainWindow(settings::m_Videomodes[settings::m_currVideomode], "Fan Game!", settings::m_Fullscreen, settings::m_FpsLimit, settings::m_Vsync);
		stateSystem::add<menu>();

		while (mainWindow.getWindow().isOpen())
		{
			stateSystem::processStateChange(mainWindow);
			mainWindow.pollEvents();
			mainWindow.update();
			mainWindow.draw();
		}
	}
	catch (const std::exception& e)
	{
		std::printf("\033[1;4;31m%s\n\033[0m", e.what());
		return 1;
	}
	catch(...)
	{
		std::printf("\033[1;4;31mUnknown error, rethrowing...\n\033[0m");
		throw;
	}
	stateSystem::cleanUp();
	resourceSystem::cleanUp();
	return 0;
}