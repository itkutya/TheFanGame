#pragma once

#include "ImGUI/imgui.h"
#include "ImGUI/imgui-SFML.h"

class gui
{
public:
	gui() noexcept;
	virtual ~gui() noexcept;

	const void init(sf::RenderWindow& window) noexcept;
	const void processEvent(const sf::Event& event) noexcept;
	const void update(sf::RenderWindow& window, const sf::Time& dt) noexcept;
	const void render(sf::RenderWindow& window) noexcept;
private:
};