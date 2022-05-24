#include "animation.h"

animation::animation() noexcept
{
	this->timer.restart();
}

animation::~animation() noexcept {}

const bool animation::update(const sf::Time& speed) noexcept
{
	if (this->timer.getElapsedTime().asMicroseconds() > speed.asMicroseconds())
	{
		std::cout << this->timer.getElapsedTime().asMicroseconds() << '\n';
		this->timer.restart();
		return true;
	}
	return false;
}
