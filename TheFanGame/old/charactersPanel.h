#pragma once

#include "gui.h"

#include "window.h"
#include "network.h"
#include "profile.h"

class charactersPanel : public guistate
{
public:
	charactersPanel() noexcept;
	virtual ~charactersPanel() noexcept = default;

	virtual const void init(window& window) noexcept;
	virtual const void processEvent(const sf::Event& event) noexcept;
	virtual const void update(window& window, const sf::Time& dt) noexcept override;
	virtual const void draw(window& window) noexcept;
private:
	struct talents
	{
		std::string m_name = "Some randrom name..." + std::to_string(std::rand() % 1000);
		bool unlocked = false;
		std::uint32_t level = 1;
		std::uint32_t price = 690;
	}; std::vector<talents> characters;
};