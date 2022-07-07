#include <exception>

#include "gui.h"

int main()
{
	//Uses the current time to generate random numbers.
	std::srand((std::uint32_t)std::time(0));

	try
	{
		context m_context;
		m_context.m_resources.addTexture(0, "res/wolftextures.png");
		m_context.m_resources.addTexture(1, "res/char.png");

		window mainWindow;
		mainWindow.create(sf::VideoMode(600, 600), "Project stuff");
		mainWindow.setContext(m_context);
		mainWindow.addState<gui>();
		mainWindow.setFramerateLimit(60);

		while (mainWindow)
		{
			mainWindow.processStateChange(mainWindow);
			mainWindow.pollEvents();
			mainWindow.update();
			mainWindow.draw();
		}
	}
	catch (const std::exception& e)
	{
		//Catch std::exceptions and return with -1.
		std::cout << "\033[1;4;31m" << e.what() << '\n' << "\033[0m";
		return -1;
	}
	catch(...)
	{
		//Catch any exception and return with -2.
		std::cout << "\033[1;4;31m" << "Something went terribly wrong...\n" << "\033[0m";
		return -2;
	}
	//Exit the program with a success.
	return 0;
}