#include "account.h"

account::account() noexcept
{
	//Log in and load values...
    std::ifstream profile("AccountInfo.txt");
    if (profile.is_open())
    {
        profile >> this->account_name >> this->xp >> this->xp_cap >> this->account_lvl >> this->currency;

        std::cout << "Account name: " << this->account_name << "\n"
            << "Account current xp: " << this->xp << "\n"
            << "Account current xp capacity: " << this->xp_cap << "\n"
            << "Account current level: " << this->account_lvl << "\n"
            << "Account currency: " << this->currency << "\n";
    }
    profile.close();
}

account::~account() noexcept
{
    //When we quit the game save everything to the file.
    std::ofstream profile("AccountInfo.txt", std::ios::trunc);
    if (profile.is_open())
    {
        profile << this->account_name << '\n' << this->xp << "\n" << this->xp_cap << '\n' << this->account_lvl << '\n' << this->currency;
        std::cout << "Saved everything to the txt, and quiting.\n";
    }
    profile.close();
}
