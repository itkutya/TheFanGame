#include "animation.h"

animation::animation() noexcept
{
	this->currentFrame = 0;
	this->timer.restart();
}

animation::~animation() noexcept {}

const bool animation::update(const sf::Time& speed, sf::VertexArray& obj, const std::uint8_t& maxFrame, const sf::FloatRect& pos) noexcept
{
	obj[0].texCoords = sf::Vector2f((float)(this->currentFrame * texWidth + pos.left), pos.top);
	obj[1].texCoords = sf::Vector2f((float)(this->currentFrame * texWidth + pos.width), pos.top);
	obj[2].texCoords = sf::Vector2f((float)(this->currentFrame * texWidth + pos.width), pos.height);
	obj[3].texCoords = sf::Vector2f((float)(this->currentFrame * texWidth + pos.left), pos.height);

	if (this->timer.getElapsedTime().asSeconds() > speed.asSeconds())
	{
		++this->currentFrame;

		if (this->currentFrame >= maxFrame)
			this->currentFrame = 0;

		this->timer.restart();
		return true;
	}
	return false;
}
