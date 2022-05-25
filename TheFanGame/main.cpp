#include <exception>

#include "window.h"

int main()
{
	window mainWindow(sf::VideoMode(500, 500), "TITLE!5!%!");
	mainWindow.setFramerateLimit(60);

	try 
	{
		while (mainWindow) 
		{
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