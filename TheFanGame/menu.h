#pragma once

#include <stdlib.h>
#include <stdio.h>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/System.hpp"
		#include "SFML64/Network.hpp"
	#else
		#include "SFML32/System.hpp"
		#include "SFML32/Network.hpp"
	#endif
#endif

#include "game.h"
#include "profile.h"

class menu : public state
{
public:
	//Construct a GUI interface.
	menu(window& window) noexcept;
	//Default destructor.
	virtual ~menu() noexcept;
	//Init a GUI interface.
	virtual const void init(sf::RenderWindow& window) override;
	//Process events on the GUI interface.
	virtual const void processEvent(const sf::Event& event) noexcept override;
	//Update the GUI interface.
	virtual const void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
	//Draw the GUI interface.
	virtual const void draw(sf::RenderWindow& window) noexcept override;
private:
	window* m_window;
	sf::View m_view;

	enum class state
	{
		Login = -1, MainMenu = 0, Settings, Characters, Singleplayer, Multiplayer, MultiLobby
	};
	state m_State = state::MainMenu;
	bool m_PlaySelected = false;
	bool m_ServerError = false;
	bool m_ChangeKeybindigs = false;
	std::pair<const std::string*, const m_Keys*> m_ToChange;
	m_Keys key = m_Keys();

	enum class settingState
	{
		Graphics = 0, Game, Audio, Mainmenu, Input, Profile
	};settingState m_SettingState = settingState::Graphics;

	sf::MyMusic* m_MainMusic;
	
	sf::Sprite icon;
	sf::Sprite frontImage;
	sf::RectangleShape backgroundImage;
	
	sf::RectangleShape xp_bar;
	sf::RectangleShape curr_xp;
	profile myAccount;

	sf::TcpSocket socket;
	sf::IpAddress serverIP = sf::IpAddress::getLocalAddress();
	sf::Uint16 serverPort = 52420;

	char InputIp[12] = "";
	char InputPort[6] = "";

	std::vector<std::pair<sf::IpAddress, sf::Uint16>> servers;
	std::uint32_t activeServerNum;

	void startServer();
	const bool shutdownServer();
	void refreshServerList();
	std::unique_ptr<sf::Thread> serverThread;
	sf::TcpListener hosting;
	sf::SocketSelector selector;
	std::vector<sf::TcpSocket*> clients;

	std::uint32_t LocalHostPlayerNum = 0;
	std::vector<std::pair<sf::IpAddress, sf::Uint16>> localHostPlayers;
	sf::Mutex mutex;
	void updateLocalServerNum();
	std::unique_ptr<sf::Thread> handleLocalPlayerNum;

	struct talents
	{
		std::string m_name = "Some randrom name..." + std::to_string(std::rand() % 1000);
		bool unlocked = false;
		std::uint32_t level = 1;
		std::uint32_t price = 690;
	};std::vector<talents> characters;

	const void giveXP(const float& amount) noexcept;
};