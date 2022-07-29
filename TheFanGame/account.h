#pragma once

#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>

#define MAX_CHAR_SIZE 16

class account
{
public:
	account() noexcept;
	virtual ~account() noexcept;

	char account_name[MAX_CHAR_SIZE] = "Unkown";
	float xp = 0.f;
	float xp_cap = 100.f;
	int account_lvl = 1;
	std::uint32_t currency = 0;
};

