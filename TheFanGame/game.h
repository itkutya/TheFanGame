#pragma once

#include "window.h"

#include "network.h"
#include "ray.h"
#include "Timer.h"

class game : public state
{
public:
	//Construct the main game class.
	game(engine& e, window& w) noexcept;
	//Defauilt destructor.
	virtual ~game() noexcept;
	//Init main game class.
	virtual const void init(sf::RenderWindow& window) override;
	//Process event in the main game class.
	virtual const void processEvent(const sf::Event& event) noexcept override;
	//Update the main game class.
	virtual const void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
	//Draw the main game objects.
	virtual const void draw(sf::RenderWindow& window) noexcept override;
private:
	engine& m_engine;
	window& m_window;
	world m_map;

	sf::View m_view;
	const sf::Texture* m_texture;
	const sf::Texture* m_char;

	std::unique_ptr<player> m_player;
	std::unique_ptr<ray> m_ray;
	std::vector<entity> m_entities;

	std::vector<float> zBuffer;
	std::vector<int> spriteOrder;
	std::vector<float> spriteDistance;

	const void sortSprites(std::vector<int>& order, std::vector<float>& dist, const std::size_t& amount) noexcept;
};

