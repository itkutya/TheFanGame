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
	world m_map;
	player m_player;
	ray m_ray = ray(600);

	sf::View m_view;
	const sf::Texture* m_texture;

	sf::VertexArray m_sprites;
	std::vector<entity> m_entities;
	std::vector<float> zBuffer;

	const void sortSprites(std::vector<int>& order, std::vector<float>& dist, const std::size_t& amount) noexcept;
};

