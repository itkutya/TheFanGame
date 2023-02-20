#pragma once

#include "gui.h"

#include "window.h"
#include "network.h"
#include "profile.h"

#include "mainmenuPanel.h"

class loginPanel : public guistate
{
public:
	loginPanel() noexcept = default;
	virtual ~loginPanel() noexcept = default;

	virtual const void init(window& window) noexcept;
	virtual const void processEvent(const sf::Event& event) noexcept;
	virtual const void update(window& window, const sf::Time& dt) noexcept override;
	virtual const void draw(window& window) noexcept;
private:
	const bool login(const std::string& name, const std::string& password) noexcept;
	const bool createAccount(const std::string& name, const std::string& password, const std::string& email) noexcept;

	bool createAccountPanel = false;
	std::string createAccountEmail;
	std::string createAccountName;
	std::string createAccountPassword;
};