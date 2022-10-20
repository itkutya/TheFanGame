#include <exception>

#include "menu.h"

int main()
{
	std::srand((std::uint32_t)std::time(0));
	try
	{
		resourceSystem rs;
		inputSystem is;
		stateSystem ss;
		settings s("res/Settings.ini");
		engine e = engine(&rs, &is, &ss, &s);

		window mainWindow(s.m_Videomodes[s.m_currVideomode], "Fan Game!", s.m_Fullscreen, s.m_FpsLimit, s.m_Vsync, e);
		ss.add<menu>(e, mainWindow);

		while (mainWindow)
		{
			ss.processStateChange(mainWindow);
			mainWindow.pollEvents();
			mainWindow.update();
			mainWindow.draw();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "\033[1;4;31m" << e.what() << '\n' << "\033[0m";
		return -1;
	}
	catch(...)
	{
		std::cout << "\033[1;4;31m" << "Something went terribly wrong...\n" << "\033[0m";
		return -2;
	}
	return 0;
}