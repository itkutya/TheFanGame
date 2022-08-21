#include <exception>

#include "menu.h"

int main()
{
	//Uses the current time to generate random numbers.
	std::srand((std::uint32_t)std::time(0));

	inputManager::init();

	resourceManager::add<sf::Texture>("WallTexture", "res/wolftextures.png");
	resourceManager::add<sf::Texture>("Background", "res/MainMenu/Backgrounds.png");
	resourceManager::add<sf::Texture>("FrontImage", "res/MainMenu/FontImages.png");
	resourceManager::add<sf::Texture>("Icon", "res/MainMenu/Icons.png");
	resourceManager::add<sf::Texture>("CharacterTexture", "res/char.png");
	resourceManager::add<sf::Font>("JP_Font", "res/Gen Jyuu Gothic Monospace Bold.ttf");
	resourceManager::add<sf::MyMusic>("Blackbird - Cecile Corbel", "res/MainMenu/Blackbird - Cecile Corbel.wav");
	resourceManager::add<sf::MyMusic>("Sakakibara Yui - Koi no Honoo", "res/MainMenu/Sakakibara Yui - Koi no Honoo.wav");

	try
	{
		window mainWindow;
		mainWindow.create(sf::VideoMode(1920, 1080), "Project stuff");
		mainWindow.setFramerateLimit(60);

		stateSystem::add<menu>(mainWindow);
		while (mainWindow)
		{
			stateSystem::processStateChange(mainWindow);
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
	//Clear the resources.
	stateSystem::clear();
	resourceManager::clear();
	//Exit the program with a success.
	return 0;
}