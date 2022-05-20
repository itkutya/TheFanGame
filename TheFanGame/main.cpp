#include <exception>
#include <iostream>

int main()
{
	try
	{

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