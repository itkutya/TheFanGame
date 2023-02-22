#include "Account.h"

Account& Account::getInstance()
{
	static Account instance;
	return instance;
}

void Account::Login() noexcept
{
}

void Account::Register() noexcept
{
}