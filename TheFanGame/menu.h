#pragma once

#include "game.h"
#include "profile.h"

#include "particleSystem.h"

class menu : public state
{
public:
	menu() noexcept;
	virtual ~menu() noexcept;
	virtual const void init(window& window) override;
	virtual const void processEvent(const sf::Event& event) noexcept override;
	virtual const void update(window& window, const sf::Time& dt) noexcept override;
	virtual const void draw(window& window) noexcept override;
private:
	//TODO:
	//Make current panels into modals...
	//World editor, loading etc...
	//ERROR handleing system
	//Audio system, Music, etc...
	//Finish the menu
	//etc...
	const void mainmenuPanel(window& window, const sf::Time& dt) noexcept;
	const void loginPanel(window& window, const sf::Time& dt) noexcept;

	bool settingsPopUpShouldBeOpen = false;
	const void settingsPanel(window& window, const sf::Time& dt) noexcept;
	const void multiplayerPanel(window& window, const sf::Time& dt) noexcept;
	bool characterPopUpShouldBeOpen = false;
	const void charactersPanel(window& window, const sf::Time& dt) noexcept;

	const bool login(const std::string& name, const std::string& password) noexcept;
	std::string createAccountEmail;
	std::string createAccountName;
	std::string createAccountPassword;
	const bool createAccount(const std::string& name, const std::string& password, const std::string& email) noexcept;

	localhost m_localhost;
	client m_client;

	enum class state
	{
		Login = -1, MainMenu = 0, Singleplayer, Multiplayer, MultiLobby
	};state m_State = state::Login;
	bool m_PlaySelected = false;
	bool m_ServerError = false;

	enum class settingState
	{
		Graphics = 0, Game, Audio, Mainmenu, Input, Profile
	};
	sf::Music* m_MainMusic;
	
	sf::Sprite icon;
	sf::Sprite frontImage;
	sf::RectangleShape backgroundImage;

	profile myAccount;

	char InputIp[12] = "";
	char InputPort[6] = "";
	std::vector<std::pair<sf::IpAddress, std::uint16_t>> servers;
	std::uint32_t activeServerNum;

	struct talents
	{
		std::string m_name = "Some randrom name..." + std::to_string(std::rand() % 1000);
		bool unlocked = false;
		std::uint32_t level = 1;
		std::uint32_t price = 690;
	};std::vector<talents> characters;

	const void giveXP(const float& amount) noexcept;
};