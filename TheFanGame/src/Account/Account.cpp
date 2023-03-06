#include "Account.h"

Account& Account::getInstance()
{
	static Account instance;
	return instance;
}