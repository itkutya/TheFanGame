#include <exception>
#include <iostream>

#include "window.h"

int main()
{
	window mainWindow(sf::VideoMode(500, 500), "TITLE!5!%!");

	try
	{
		while (mainWindow)
		{
			mainWindow.pollEvents();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "\033[1;4;31m" << e.what() << '\n' << "\033[0m";
	}
	catch(...)
	{
		std::cout << "\033[1;4;31m" << "Something went terribly wrong...\n" << "\033[0m";
	}

	return 0;
}