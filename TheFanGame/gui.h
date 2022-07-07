#pragma once

#include "game.h"

class gui : public state
{
public:
	//Construct a GUI interface.
	gui(window& window) noexcept;
	//Default destructor.
	virtual ~gui() noexcept;
	//Init a GUI interface.
	virtual const void init(sf::RenderWindow& window) override;
	//Process events on the GUI interface.
	virtual const void processEvent(const sf::Event& event) noexcept override;
	//Update the GUI interface.
	virtual const void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
	//Draw the GUI interface.
	virtual const void draw(sf::RenderWindow& window) noexcept override;
private:
	window* m_window;
	float xp = 0.f;
	bool settingsPanel = false;
	sf::Sprite sprite;
	
	//Creates a button, return's true if it's pressed, false if not.
	const bool createButton(const char* name, const sf::Vector2f& pos, const sf::Vector2f& size);
	//Creates an image.
	const void createImage(const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& pos, const sf::Vector2f& size);
};