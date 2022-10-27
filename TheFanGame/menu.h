#pragma once

#include "game.h"
#include "profile.h"

class menu : public state
{
public:
	//Construct a GUI interface.
	menu(engine& e, window& w) noexcept;
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
	engine& m_engine;
	window& m_window;
	sf::View m_view;

	localhost m_localhost;
	client m_client;

	enum class state
	{
		Login = -1, MainMenu = 0, Settings, Characters, Singleplayer, Multiplayer, MultiLobby
	};state m_State = state::Login;
	bool m_PlaySelected = false;
	bool m_ServerError = false;
	bool m_ChangeKeybindigs = false;
	std::pair<std::string, m_Keys> m_ToChange;
	m_Keys key = m_Keys();

	enum class settingState
	{
		Graphics = 0, Game, Audio, Mainmenu, Input, Profile
	};settingState m_SettingState = settingState::Graphics;

	sf::MyMusic* m_MainMusic;

	ImFont* font;
	
	sf::Sprite icon;
	sf::Sprite frontImage;
	sf::RectangleShape backgroundImage;
	
	sf::RectangleShape xp_bar;
	sf::RectangleShape curr_xp;
	profile myAccount;

	std::string inputName;
	std::string inputPW;
	bool logged_in = false;
	bool rememberToStayLogedIn = false;
	const void login() noexcept;
	const void createAccount() noexcept;

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