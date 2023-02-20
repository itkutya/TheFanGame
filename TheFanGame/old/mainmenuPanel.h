#pragma once

#include "gui.h"
#include "window.h"

#include "loginPanel.h"
#include "settingsPanel.h"
#include "multiplayerPanel.h"
#include "charactersPanel.h"

class mainmenuPanel : public guistate
{
public:
	mainmenuPanel() noexcept;
	virtual ~mainmenuPanel() noexcept = default;

	virtual const void init(window& window) noexcept;
	virtual const void processEvent(const sf::Event& event) noexcept;
	virtual const void update(window& window, const sf::Time& dt) noexcept override;
	virtual const void draw(window& window) noexcept;
private:
	bool m_PlaySelected = false;

	settingsPanel sPanel;
	multiplayerPanel mpPanel;

	sf::Music* m_MainMusic = nullptr;
};