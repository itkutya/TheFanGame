#include <exception>

#include "src/Application.h"

int main()
{
	try
	{
		Application app("TheFanGame");
		while (app.getWindow().isOpen())
		{
			app.pollEvents();
			app.update();
			app.draw();
		}
	}
	catch (std::exception e)
	{
		std::printf("\033[1;4;31m%s\n\033[0m", e.what());
		return 1;
	}
	catch(...)
	{
		std::printf("\033[1;4;31mUnknown error!\n\033[0m");
		return 2;
	}
	return 0;
}