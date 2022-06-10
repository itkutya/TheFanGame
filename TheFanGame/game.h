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

	const void sortSprites(std::vector<int>& order, std::vector<float>& dist, const std::size_t& amount) noexcept;
private:
	context* m_context;
	world m_Map;
	player m_Player;
	ray m_Ray;

	sf::View m_view;
	sf::RenderStates m_state;

	sf::VertexArray m_walls;
	sf::VertexArray m_sprites;
	std::vector<entity> m_entities;
	std::vector<float> zBuffer;
};

