#include "animation.h"

animation::animation() 
{
	this->timer.restart();
}

animation::~animation() {}

const bool animation::update(const float& dt, const float& speed)
{
	if (this->timer.getElapsedTime().asSeconds() > speed * dt)
	{
		std::cout << this->timer.getElapsedTime().asSeconds();
		this->timer.restart();
		return true;
	}
	return false;
}
