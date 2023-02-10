#pragma once

#include "includes.h"
#include "window.h"

class settingsPanel
{
public:
	settingsPanel() noexcept = default;
	virtual ~settingsPanel() noexcept = default;

	const void upadte(window& window, const sf::Time& dt) noexcept;

	bool settingsPopUpShouldBeOpen = false;
	sf::Music* m_MainMusic = nullptr;
	sf::Sprite* icon = nullptr;
	sf::Sprite* frontImage = nullptr;
	sf::Sprite* backgroundImage = nullptr;
private:
	enum class settingStates
	{
		Graphics = 0, Game, Audio, Mainmenu, Input, Profile
	};
	settingStates state = settingStates::Graphics;
};