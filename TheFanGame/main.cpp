#include <exception>
#include <iostream>

int main()
{
	try
	{

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	catch(...)
	{
		std::cout << "Somehing went wrong...\n";
	}
	return 0;
}