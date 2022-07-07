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
	sf::Sprite sprite;
	float xp = 0.f;
	bool settingsPanel = false;
	std::vector<sf::VideoMode> m_videomodes;
	bool fullscreen = false;
	bool isFPSLimited = true;
	int fps_limit = 60;
	float sensivity = 2.5f;
	float game_volume = 100.f;
	float music_volume = 100.f;

	//Creates a button, return's true if it's pressed, false if not.
	const bool createButton(const char* name, const sf::Vector2f& pos, const sf::Vector2f& size) noexcept;
	//Creates an image.
	const void createImage(const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& pos, const sf::Vector2f& size) noexcept;
	//Set's a tooltip when you hover over an imgui item, use it after you created an imgui item.
	const bool setToolTip(const char* text) noexcept;
};