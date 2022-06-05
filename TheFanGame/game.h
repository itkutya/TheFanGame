#pragma once

#include "ImGUI/imgui.h"
#include "ImGUI/imgui-SFML.h"

#include "stateSystem.h"
#include "ray.h"

struct context
{
	stateSystem g_states;
	resourceManager g_resources;
};

class game : public state
{
public:
	game(context* context) noexcept;
	virtual ~game() noexcept;

	virtual const void init(sf::RenderWindow& window) override;
	virtual const void processEvent(const sf::Event& event) noexcept override;
	virtual const void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
	virtual const void draw(sf::RenderWindow& window) noexcept override;
private:
	context* m_context;
	world miniMap;
	player miniPlayer;
	ray playerRay;

	sf::VertexArray walls;
	sf::RenderStates state;
	sf::View view;
};

