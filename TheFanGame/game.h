#pragma once

#include "stateSystem.h"
#include "player.h"

class game : public state
{
public:
	game() noexcept;
	virtual ~game() noexcept;

	virtual const void init(sf::RenderWindow& window) override;
	virtual const void processEvent(const sf::Event& event) noexcept override;
	virtual const void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
	virtual const void draw(sf::RenderWindow& window) noexcept override;
private:
};

