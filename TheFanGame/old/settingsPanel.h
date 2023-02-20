#pragma once

#include "gui.h"
#include "window.h"

class settingsPanel : public guistate
{
public:
	settingsPanel() noexcept;
	virtual ~settingsPanel() noexcept = default;

	virtual const void init(window& window) noexcept;
	virtual const void processEvent(const sf::Event& event) noexcept;
	virtual const void update(window& window, const sf::Time& dt) noexcept override;
	virtual const void draw(window& window) noexcept;

	sf::Sprite m_icon;
	sf::Sprite m_frontImage;
	inline static sf::Sprite m_backgroundImage;
private:
	enum class settingStates
	{
		Graphics = 0, Game, Audio, Mainmenu, Input, Profile
	};
	settingStates state = settingStates::Graphics;

	sf::Music* m_MainMusic = nullptr;
};