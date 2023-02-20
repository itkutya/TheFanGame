#pragma once

#include "window.h"

#include "network.h"
#include "ray.h"
#include "Timer.h"

class game : public state
{
public:
	game() noexcept;
	virtual ~game() noexcept;

	virtual const void init(window& window) override;
	virtual const void processEvent(const sf::Event& event) noexcept override;
	virtual const void update(window& window, const sf::Time& dt) noexcept override;
	virtual const void draw(window& window) noexcept override;
private:
	world m_map;

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

