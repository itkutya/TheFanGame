#pragma once

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
#include "account.h"

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

	enum class settingState
	{
		Graphics = 0, Game, Audio, Mainmenu
	};settingState m_SettingState = settingState::Graphics;

	sf::Sound MainMusic;

	const bool saveSettings(const std::string& filePath) const noexcept;
	const bool loadSettings(const std::string& filePath) noexcept;

	/*Game settings -> different class... / maybie part of context?*/
	std::vector<sf::VideoMode> m_videomodes;
	bool fullscreen = false;
	bool isFPSLimited = true;
	bool m_ShowFPS = true;
	int fps_limit = 60;
	float sensivity = 2.5f;
	float game_volume = 100.f;
	float music_volume = 100.f;
	int currProfilePicture = 0;
	int currFrontPicture = 0;
	int currBackgroundPicture = 0;

	sf::Sprite profilePicture;
	sf::Sprite frontPicture;
	sf::RectangleShape backgroundImage;
	
	sf::RectangleShape xp_bar;
	sf::RectangleShape curr_xp;
	account myAccount;

	sf::TcpSocket socket;
	sf::IpAddress serverIP = sf::IpAddress::getLocalAddress();
	sf::Uint16 serverPort = 52420;

	char InputIp[12] = "";
	char InputPort[6] = "";

	std::vector<std::pair<sf::IpAddress, sf::Uint16>> servers;
	std::uint32_t activeServerNum;

	void startServer();
	void shutdownServer();
	void refreshServerList();
	std::unique_ptr<sf::Thread> serverThread;
	sf::TcpListener hosting;
	std::vector<sf::TcpSocket*> clients;
	sf::SocketSelector selector;

	struct talents
	{
		std::string m_name = "Some randrom name..." + std::to_string(std::rand() % 1000);
		bool unlocked = false;
		std::uint32_t level = 1;
		std::uint32_t price = 690;
	};std::vector<talents> characters;

	const void giveXP(const float& amount) noexcept;
};