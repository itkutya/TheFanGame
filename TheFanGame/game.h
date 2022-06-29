#pragma once

#include "ImGUI/imgui.h"
#include "ImGUI/imgui-SFML.h"

#include "stateSystem.h"
#include "ray.h"

#include "Timer.h"

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

	sf::View m_view;
	const sf::Texture* m_texture;

	std::unique_ptr<player> m_player;
	std::unique_ptr<ray> m_ray;
	std::vector<entity> m_entities;

	std::vector<float> zBuffer;
	std::vector<int> spriteOrder;
	std::vector<float> spriteDistance;

	const void sortSprites(std::vector<int>& order, std::vector<float>& dist, const std::size_t& amount) noexcept;
};

