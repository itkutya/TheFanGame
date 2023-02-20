#pragma once

#include "gui.h"

#include "window.h"
#include "network.h"

class multiplayerPanel : public guistate
{
public:
	multiplayerPanel() noexcept = default;
	virtual ~multiplayerPanel() noexcept = default;

	virtual const void init(window& window) noexcept;
	virtual const void processEvent(const sf::Event& event) noexcept;
	virtual const void update(window& window, const sf::Time& dt) noexcept override;
	virtual const void draw(window& window) noexcept;


	std::uint32_t ServerNum = 0;
	std::vector<std::pair<sf::IpAddress, std::uint16_t>> servers;
	client network;
	localhost host;
private:
	enum class state {
		Local, Public, Private, Unknown
	}; state m_state = state::Public;
	std::string InputIp = "";
	std::string InputPort = "";

	const void refresLocal() noexcept;
	const void refresPublic() noexcept;
};