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

	enum class panels
	{
		mainmenu = 0, settings, characters, singleplayer, multiplayer, multilobby
	};

	panels curr_panel = panels::mainmenu;
	bool play_selecter = false;

	std::vector<sf::VideoMode> m_videomodes;
	bool fullscreen = false;
	bool isFPSLimited = true;
	int fps_limit = 60;
	float sensivity = 2.5f;
	float game_volume = 100.f;
	float music_volume = 100.f;

	char buffer[12] = "";

	sf::RectangleShape backgroundImage;
	int currProfilePicture = 0;
	int currFrontPicture = 0;
	int currBackgroundPicture = 0;

	sf::RectangleShape xp_bar;
	sf::RectangleShape curr_xp;
	account myAccount;
	sf::TcpSocket socket;
	sf::IpAddress server = sf::IpAddress::getLocalAddress();
	sf::Uint32 port = 52420;

	std::vector<sf::IpAddress> servers;
	std::vector<std::pair<sf::IpAddress, std::uint32_t>> iport;
	std::uint32_t activeServerNum;

	struct talents
	{
		std::string m_name = "Some randrom name..." + std::to_string(std::rand() % 1000);
		bool unlocked = false;
		std::uint32_t level = 1;
	};
	std::vector<talents> characters;

	//Gives a certain amount of xp to the account.
	const void giveXP(const float& amount) noexcept;
	//Creates a button, return's true if it's pressed, false if not.
	const bool createButton(const char* name, const sf::Vector2f& pos, const sf::Vector2f& size) noexcept;
	//Creates an image.
	const void createImage(const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& pos, const sf::Vector2f& size) noexcept;
	//Set's a tooltip when you hover over an imgui item, use it after you created an imgui item.
	const bool setToolTip(const char* text) noexcept;
};