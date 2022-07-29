#pragma once

#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>

class account
{
public:
	account() noexcept;
	virtual ~account() noexcept;

	std::string account_name = "Unkown";
	float xp = 0.f;
	float xp_cap = 100.f;
	int account_lvl = 1;
	std::uint32_t currency = 0;
};

