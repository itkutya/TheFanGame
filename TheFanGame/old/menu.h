#pragma once

#include "game.h"
#include "gui.h"
#include "mainmenuPanel.h"
#include "loginPanel.h"

class menu : public state
{
public:
	menu() noexcept;
	virtual ~menu() noexcept;
	virtual const void init(window& window) override;
	virtual const void processEvent(const sf::Event& event) noexcept override;
	virtual const void update(window& window, const sf::Time& dt) noexcept override;
	virtual const void draw(window& window) noexcept override;
private:
	//TODO:
	//Make current panels into modals...
	//World editor, loading etc...
	//ERROR handleing system
	//Audio system, Music, etc...
	//Finish the menu
	//etc...

	sf::Music* m_MainMusic = nullptr;
};