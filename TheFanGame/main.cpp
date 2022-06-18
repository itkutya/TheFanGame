#include <exception>

#include "window.h"

int main()
{
	std::srand(std::time(0));

	try
	{
		context m_context;
		m_context.g_resources.addTexture(0, "res/wolftextures.png");
		window mainWindow(sf::VideoMode(600, 600), "TITLE!5!%!", &m_context);
		mainWindow.setFramerateLimit(60);

		while (mainWindow)
		{
			m_context.g_states.processStateChange(mainWindow);
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