#pragma once

#include "ImGUI/imgui.h"
#include "ImGUI/imgui-SFML.h"

#include "game.h"

class gui : public state
{
public:
	gui(std::unique_ptr<stateSystem>& context) noexcept;
	virtual ~gui() noexcept;

	virtual const void init(sf::RenderWindow& window) override;
	virtual const void processEvent(const sf::Event& event) noexcept override;
	virtual const void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
	virtual const void draw(sf::RenderWindow& window) noexcept override;
private:
	stateSystem* m_context;
	sf::Sprite sprite;
	float xp = 0.f;
};