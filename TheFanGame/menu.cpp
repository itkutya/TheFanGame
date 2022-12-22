#include "menu.h"

menu::menu() noexcept
{
	resourceSystem::add<sf::Music>("Blackbird - Cecile Corbel", "res/MainMenu/Blackbird - Cecile Corbel.wav");
	resourceSystem::add<sf::Music>("Sakakibara Yui - Koi no Honoo", "res/MainMenu/Sakakibara Yui - Koi no Honoo.wav");
	resourceSystem::add<sf::Texture>("WallTexture", "res/wolftextures.png");
	resourceSystem::add<sf::Texture>("Background", "res/MainMenu/Backgrounds.png");
	resourceSystem::add<sf::Texture>("FrontImage", "res/MainMenu/FontImages.png");
	resourceSystem::add<sf::Texture>("Icon", "res/MainMenu/Icons.png");
	resourceSystem::add<sf::Texture>("Pause", "res/MainMenu/Pause.png");
	resourceSystem::add<sf::Texture>("Resume", "res/MainMenu/Resume.png");
	resourceSystem::add<sf::Texture>("CharacterTexture", "res/char.png");
	resourceSystem::add<sf::Font>("JP_Font", "res/Gen Jyuu Gothic Monospace Bold.ttf");
	resourceSystem::wait();
}

menu::~menu() noexcept 
{
	this->m_MainMusic->stop();
	ImGui::SFML::Shutdown();
}

const void menu::init(window& window)
{
	ImGui::SFML::Init(window.getWindow());
	gui::loadStyle();
	gui::loadFont();

	//Load characters...
	for (std::size_t i = 0; i < 5; ++i)
		this->characters.emplace_back();

	this->frontImage.setTexture(resourceSystem::c_get<sf::Texture>("FrontImage"));
	this->frontImage.setTextureRect(sf::IntRect(sf::Vector2i(600 * settings::m_currFrontPicture, 0), sf::Vector2i(600, 600)));

	this->icon.setTexture(resourceSystem::c_get<sf::Texture>("Icon"));
	this->icon.setTextureRect(sf::IntRect(sf::Vector2i(100 * settings::m_currProfilePicture, 0), sf::Vector2i(100, 100)));

	this->backgroundImage.setTexture(&resourceSystem::c_get<sf::Texture>("Background"));
	this->backgroundImage.setTextureRect(sf::IntRect({ 1920 * settings::m_currBackgroundPicture, 0 }, { 1920, 1080 }));
	this->backgroundImage.setSize(window.getView().getSize());
	this->backgroundImage.setFillColor(sf::Color(255, 255, 255, 200));

	this->m_MainMusic = resourceSystem::get<std::unique_ptr<sf::Music>>(settings::m_currMusic).get();
	this->m_MainMusic->setLoop(true);
	if (settings::m_MusicVolume > 0.f)
		this->m_MainMusic->play();
	this->m_MainMusic->setVolume(settings::m_MusicVolume);

	if (settings::rememberToStayLogedIn)
		this->m_State = state::MainMenu;
}

const void menu::update(window& window, const sf::Time& dt) noexcept
{
	ImGui::SFML::Update(window.getWindow(), dt);

	panel mainWindowPanel("Main Window", window.getWindow());

	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();
	vMin.x += ImGui::GetWindowPos().x;
	vMin.y += ImGui::GetWindowPos().y;
	vMax.x += ImGui::GetWindowPos().x;
	vMax.y += ImGui::GetWindowPos().y;

	if (settings::m_ShowFPS)
	{
		ImGui::SetCursorPos(ImVec2(vMax.x * 0.85f, vMin.y));
		auto& io = ImGui::GetIO();
		ImGui::Text("%.2f fps (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
	}

	switch (this->m_State)
	{
	case state::MainMenu:
	{
		this->mainmenuPanel(window, dt);
		break;
	}
	case state::Multiplayer:
	{
		panel MultiplayerPanel("Multiplayer", window.getWindow());

		vMin = ImGui::GetWindowContentRegionMin();
		vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		if (ImGui::BeginListBox("Servers", ImVec2(ImGui::GetWindowSize().x - 100.f, ImGui::GetWindowSize().y - 275.f)))
		{
			for (std::size_t i = 0; i < this->servers.size(); ++i)
			{
				std::string serverInfo = this->servers[i].first.toString() + ':' + std::to_string(this->servers[i].second);
				if (ImGui::Selectable(serverInfo.c_str()))
				{
					std::cout << "connecting to the server... " << this->servers[i].first.toString() << ":" << this->servers[i].second << "\n";
					/*
					this->socket.disconnect();
					if (this->socket.connect(this->servers[i].first, this->servers[i].second) == sf::Socket::Done)
					{
						sf::Packet packet;
						if (this->socket.receive(packet) == sf::Socket::Done)
						{
							packet >> this->LocalHostPlayerNum;
							packet.clear();
							for (std::size_t i = 0; i < this->LocalHostPlayerNum; ++i)
							{
								if (this->socket.receive(packet) == sf::Socket::Done)
								{
									std::string ip;
									sf::Uint16 g_port;
									packet >> ip >> g_port;
									this->localHostPlayers.push_back(std::pair<sf::IpAddress, sf::Uint16>(sf::IpAddress(ip), g_port));
									std::cout << this->localHostPlayers[i].first.toString() << ":" << this->localHostPlayers[i].second << '\n';
								}
								packet.clear();
							}
							std::cout << "Succesfuly connected to the server...\n";
							this->m_State = state::MultiLobby;
							this->handleLocalPlayerNum = std::make_unique<sf::Thread>(&menu::updateLocalServerNum, this);
							this->handleLocalPlayerNum->launch();
						}
					}
					else
					{
						this->socket.connect(this->serverIP, this->serverPort);
						this->m_ServerError = true;
					}
					*/
				}
			}
			ImGui::EndListBox();
		}

		if (ImGui::Button("Refress"))
			//this->refreshServerList();

			ImGui::SetCursorPos(ImVec2(vMin.x, vMax.y - 175.f));
		ImGui::Text("Server IP: ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(window.getWindow().getSize().x / 5.f);
		if (ImGui::InputTextWithHint("##ServerIP", "127.0.0.0", this->InputIp, 12, ImGuiInputTextFlags_EnterReturnsTrue))
			std::cout << this->InputIp << "\n";
		ImGui::SameLine();
		ImGui::Text("Port: ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(window.getWindow().getSize().x / 8.f);
		if (ImGui::InputTextWithHint("##Port", "52420", this->InputPort, 6, ImGuiInputTextFlags_EnterReturnsTrue))
			std::cout << this->InputPort << "\n";
		ImGui::SameLine();
		ImVec2 WidgetPos = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(WidgetPos.x, WidgetPos.y - 10.f));
		if (ImGui::Button("Join", ImVec2(200.f, 50.f)))
		{
			std::cout << "connecting to the server... " << this->InputIp << ":" << this->InputPort << "\n";
			/*
			this->socket.disconnect();
			if (this->socket.connect(sf::IpAddress(this->InputIp), std::uint16_t(std::atoi(this->InputPort))) == sf::Socket::Done)
			{
				sf::Packet packet;
				if (this->socket.receive(packet) == sf::Socket::Done)
				{
					packet >> this->LocalHostPlayerNum;
					packet.clear();
					for (std::size_t i = 0; i < this->LocalHostPlayerNum; ++i)
					{
						if (this->socket.receive(packet) == sf::Socket::Done)
						{
							std::string ip;
							sf::Uint16 g_port;
							packet >> ip >> g_port;
							this->localHostPlayers.push_back(std::pair<sf::IpAddress, sf::Uint16>(sf::IpAddress(ip), g_port));
							std::cout << this->localHostPlayers[i].first.toString() << ":" << this->localHostPlayers[i].second << '\n';
						}
						packet.clear();
					}
					std::cout << "Succesfuly connected to the server...\n";
					this->m_State = state::MultiLobby;
				}
			}
			else
			{
				this->socket.connect(this->serverIP, this->serverPort);
				this->m_ServerError = true;
			}
			*/
		}

		ImGui::SetCursorPos(ImVec2(vMin.x + 25.f, vMax.y - 125.f));
		if (ImGui::Button("Host", ImVec2(300.f, 75.f)))
			std::printf("Will fix it soonTM...\n");

		ImGui::SetCursorPos(ImVec2(vMax.x - 350.f, vMax.y - 125.f));
		if (ImGui::Button("Back##Multiplayer", ImVec2(300.f, 75.f)))
			this->m_State = state::MainMenu;
		break;
	}
	case state::Singleplayer:
	{
		panel SinglePlayerPanel("Singleplayer", window.getWindow());
		break;
	}
	case state::MultiLobby:
	{
		panel MultiLobbyPanel("MultiLobby", window.getWindow());
		vMin = ImGui::GetWindowContentRegionMin();
		vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		ImGui::Text("Connected players: ");
		ImGui::SameLine();
		if (ImGui::BeginListBox("##PlayerList", ImVec2(ImGui::GetWindowSize().x - 500.f, ImGui::GetWindowSize().y - 200.f)))
		{
			/*
			sf::Lock lock(this->mutex);
			for (std::size_t i = 0; i < this->localHostPlayers.size(); ++i)
			{
				std::string clientInfo = this->localHostPlayers[i].first.toString() + ":" + std::to_string(this->localHostPlayers[i].second);
				if (ImGui::Selectable(clientInfo.c_str()))
				{
					//Mute
					// if admin...
					//	Kick
					//	Ban
				}
			}
			*/
			ImGui::EndListBox();
		}
		ImGui::SameLine();
		if (ImGui::BeginChild("Game settings", ImVec2(0, ImGui::GetWindowContentRegionMax().y - 200.f), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
		{
			bool teszt = false;
			if (ImGui::Checkbox("Teszt option", &teszt))
				std::cout << "Teszt\n";
			if (ImGui::Checkbox("Even More Teszt option", &teszt))
				std::cout << "Teszt\n";
			if (ImGui::Checkbox("Teamdmg or something...", &teszt))
				std::cout << "Teszt\n";
			if (ImGui::BeginCombo("Game modes maybie?", "First Game mode name"))
			{
				for (std::size_t i = 0; i < 5; ++i)
				{
					ImGui::PushID("GameModes" + i);
					if (ImGui::Selectable("Stuff"))
						std::cout << "Ok!\n";
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}
			//Switch case maybie??? idk...
			ImGui::Text("GameModeTextGoesHere");
			if (ImGui::BeginCombo("Map maybie?", "MapID"))
			{
				for (std::size_t i = 0; i < 5; ++i)
				{
					ImGui::PushID("Maps" + i);
					if (ImGui::Selectable("Stuff"))
						std::cout << "Ok!\n";
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}
			//Switch case maybie??? idk...
			ImGui::Text("MapTextGoesHere");
		}ImGui::EndChild();

		ImGui::SetCursorPos(ImVec2(vMin.x + 25.f, vMax.y - 125.f));
		if (ImGui::Button("Start##Lobby", ImVec2(300.f, 75.f)))
			std::cout << "Staring lobby...\n";

		ImGui::SetCursorPos(ImVec2(vMax.x - 350.f, vMax.y - 125.f));
		if (ImGui::Button("Back##MultiLobby", ImVec2(300.f, 75.f)))
		{
			/*
			if (this->shutdownServer())
				std::cout << "Server succesfully shut down...\n";
			else
			{
				//Client disconnects since there is no server thread...
				this->handleLocalPlayerNum->terminate();
				this->handleLocalPlayerNum.release();

				sf::Packet packet;
				packet << std::uint8_t(52);
				if (this->socket.send(packet) != sf::Socket::Done)
					std::cout << "Error! Cannot send quit msg to the server...\n";
				packet.clear();

				this->socket.disconnect();
				this->socket.connect(this->serverIP, this->serverPort);
			}
			this->localHostPlayers.clear();
			this->LocalHostPlayerNum = 0;
			this->refreshServerList();
			*/
			this->m_State = state::Multiplayer;
		}
		break;
	}
	case state::Login:
	{
		this->loginPanel(window, dt);
		break;
	}
	default:
		break;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
	ImGui::RenderNotifications();
	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(1);
}

const void menu::processEvent(const sf::Event& event) noexcept 
{ 
	ImGui::SFML::ProcessEvent(event);
	if (event.type == sf::Event::Resized)
		this->backgroundImage.setSize(sf::Vector2f((float)event.size.width, (float)event.size.height));
}

const void menu::draw(window& window) noexcept 
{
	window.getWindow().setView(window.getView());
	window.getWindow().draw(this->backgroundImage);
	window.getWindow().setView(window.getWindow().getDefaultView());
	ImGui::SFML::Render(window.getWindow());
}

const void menu::mainmenuPanel(window& window, const sf::Time& dt) noexcept
{
	if (ImGui::BeginTable("MainMenu", 2))
	{
		ImGui::TableNextColumn();
		if (ImGui::BeginTable("MainMenuProfile", 2, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableNextColumn();
			ImGui::Image(this->icon, sf::Vector2f(100.f, 100.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("This is the profile picture!");
			ImGui::TableNextColumn();
			ImGui::Text("Account: %s\nXP: %0.f / %0.f\nLevel: %i\nCoverCoin: %i$",
				this->myAccount.account_name,
				this->myAccount.xp, this->myAccount.xp_cap,
				this->myAccount.account_lvl,
				this->myAccount.currency);
			ImGui::Dummy(ImVec2(ImGui::GetContentRegionMax().x, 0.f));
			ImGui::ProgressBar(this->myAccount.xp / this->myAccount.xp_cap);
			ImGui::EndTable();
		}
		ImGui::TableNextColumn();
		if (this->m_MainMusic->getStatus() == sf::SoundSource::Playing)
		{
			if (ImGui::ImageButton(resourceSystem::c_get<sf::Texture>("Pause"), sf::Vector2f(35.f, 35.f)))
				this->m_MainMusic->pause();
		}
		else
			if (ImGui::ImageButton(resourceSystem::c_get<sf::Texture>("Resume"), sf::Vector2f(35.f, 35.f)))
				this->m_MainMusic->play();
		ImGui::SameLine();
		ImGui::Text("Currently playing: ");
		ImGui::SameLine();
		if (ImGui::BeginCombo("###MusicSelector", settings::m_currMusic.c_str(), ImGuiComboFlags_HeightSmall))
		{
			for (auto& music : settings::m_Music)
			{
				if (ImGui::Selectable(music))
				{
					if (settings::m_currMusic.c_str() != music)
					{
						settings::m_currMusic = music;
						this->m_MainMusic->stop();
						this->m_MainMusic = resourceSystem::get<std::unique_ptr<sf::Music>>(settings::m_currMusic).get();
						if (settings::m_MusicVolume > 0.f)
							this->m_MainMusic->play();
						this->m_MainMusic->setVolume(settings::m_MusicVolume);
					}
				}
			}
			ImGui::EndCombo();
		}

		ImGui::TableNextColumn();
		ImGui::Dummy(ImVec2(0.f, 100.f));

		if (ImGui::BeginTable("MainMenuButtons", 1, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableNextRow(0, 100.f);
			ImGui::TableNextColumn();
			ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 3.f);

			if (ImGui::Button("Play", ImVec2(300.f, 75.f)))
			{
				this->m_PlaySelected = true;
				ImGui::OpenPopup("PlaySelected", ImGuiPopupFlags_NoOpenOverExistingPopup);
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Start playing the game RIGHT NOW!");
			if (ImGui::BeginPopupModal("PlaySelected", &this->m_PlaySelected, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
			{
				if (ImGui::Button("Singleplayer", ImVec2(200.f, 50.f)))
				{
					//TODO:
					//Popup...
					this->m_PlaySelected = false;
					this->m_State = state::Singleplayer;
					this->m_MainMusic->stop();
					//stateSystem::add<game>(window);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Starts the game state.");

				if (ImGui::Button("Multiplayer", ImVec2(200.f, 50.f)))
					std::printf("SoonTM, again...\n");
				ImGui::EndPopup();
			}

			ImGui::TableNextRow(0, 100.f);
			ImGui::TableNextColumn();
			ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 3.f);

			if (ImGui::Button("Characters", ImVec2(300.f, 75.f)))
				this->characterPopUpShouldBeOpen = true;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Unlocked characters and the character shop, etc...");
			this->charactersPanel(window, dt);

			ImGui::TableNextRow(0, 100.f);
			ImGui::TableNextColumn();
			ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 3.f);

			if (ImGui::Button("Settings", ImVec2(300.f, 75.f)))
				this->settingsPopUpShouldBeOpen = true;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Opens the setting menu.");
			this->settingsPanel(window, dt);

			ImGui::TableNextRow(0, 100.f);
			ImGui::TableNextColumn();
			ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 3.f);

			if (ImGui::Button("Quit", ImVec2(300.f, 75.f)))
				window.getWindow().close();
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Goodbye!");
			ImGui::EndTable();
		}
		ImGui::TableNextColumn();
		ImGui::Image(this->frontImage, sf::Vector2f(ImGui::GetContentRegionAvail().x - 5.f, ImGui::GetContentRegionAvail().y - 5.f));
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("This is the front image!");
		ImGui::EndTable();
	}
}

const void menu::loginPanel(window& window, const sf::Time& dt) noexcept
{
	panel loginPanel("Login", window.getWindow());
	if (loginPanel.input("User Name:", "Enter your user name here...", settings::ProfileNickname) ||
		loginPanel.input("Password:", "Enter your password here...", settings::ProfilePassword, ImGuiInputTextFlags_Password) ||
		loginPanel.button("Login"))
		if (this->login(settings::ProfileNickname, settings::ProfilePassword))
			this->m_State = state::MainMenu;
	ImGui::SameLine();
	static bool createAccountPanel = false;
	if (loginPanel.button("Create Account"))
		createAccountPanel = true;
	ImGui::SameLine();
	ImGui::Checkbox("Remember Me!", &settings::rememberToStayLogedIn);

	popup createAccount("Create Account", window.getWindow(), createAccountPanel);
	if(createAccountPanel)
	{
		if (createAccount.input("Email:", "Enter your email here...", this->createAccountEmail) ||
			createAccount.input("Username:", "Enter your name here...", this->createAccountName) ||
			createAccount.input("Password:", "Enter your password here...", this->createAccountPassword, ImGuiInputTextFlags_Password) ||
			createAccount.button("Create"))
			if (this->createAccount(this->createAccountName, this->createAccountPassword, this->createAccountEmail))
			{
				createAccount.close();
				this->m_State = state::MainMenu;
				settings::ProfileNickname = this->createAccountName;
			}
		ImGui::SameLine();
		if (createAccount.button("Cancel"))
			createAccount.close();
	}
}

const void menu::settingsPanel(window& window, const sf::Time& dt) noexcept
{
	popup settingsPopUp("Settings", window.getWindow(), this->settingsPopUpShouldBeOpen, ImGuiWindowFlags_MenuBar);
	if (this->settingsPopUpShouldBeOpen)
	{
		static settingState settingState = settingState::Graphics;
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Mainmenu"))
				settingState = settingState::Mainmenu;
			if (ImGui::MenuItem("Profile"))
				settingState = settingState::Profile;
			if (ImGui::MenuItem("Game"))
				settingState = settingState::Game;
			if (ImGui::MenuItem("Graphics"))
				settingState = settingState::Graphics;
			if (ImGui::MenuItem("Input"))
				settingState = settingState::Input;
			if (ImGui::MenuItem("Audio"))
				settingState = settingState::Audio;
			ImGui::EndMenuBar();
		}
		switch (settingState)
		{
		case settingState::Profile:
		{
			//TODO:
			//Change name
			//Pw
			//Etc...
			break;
		}
		case settingState::Input:
		{
			inputSystem is;
			static bool changeKeyBindings = false;
			static std::pair<std::string, Input>* toChange;
			for (auto& it : is.getInputHandler())
			{
				ImGui::Text(it.first.c_str());
				ImGui::SameLine();
				ImGui::PushID(std::string(it.first + is.keyToString(it.second)).c_str());
				if (settingsPopUp.button(is.keyToString(it.second).c_str(), ImVec2(300.f, 30.f)))
				{
					toChange = &it;
					changeKeyBindings = true;
				}
				ImGui::PopID();
				ImGui::SameLine();
				ImGui::SetNextItemWidth(200.f);
				ImGui::PushID(it.first.c_str() + static_cast<std::uint32_t>(it.second.m_InputType));
				if (ImGui::BeginCombo("###PressOrRelease", is.eventToString(it.second).c_str(), ImGuiComboFlags_HeightSmall))
				{
					if (ImGui::Selectable("Press"))
						std::cout << "Later...\n";
					if (ImGui::Selectable("Release"))
						std::cout << "Later...\n";
					ImGui::EndCombo();
				}
				ImGui::PopID();
			}
			popup keyBindChanger("Change Keybindigs", window.getWindow(), changeKeyBindings);
			if (changeKeyBindings)
			{
				static Input key = Input();
				std::string text = "Change [" + is.keyToString(toChange->second) + "] to [" + is.keyToString(key) + "]";
				keyBindChanger.text(text.c_str(), ImVec4(1, 0, 0, 1));

				if (is.checkForAnyInput())
				{
					if (is.checkForAnyKeyboardInput().has_value() && is.checkForAnyKeyboardInput().value().m_KeyCode != sf::Keyboard::Enter)
						key = is.checkForAnyKeyboardInput().value();

					if (is.checkForAnyMouseInput().has_value() && is.checkForAnyMouseInput().value().m_MouseButton != sf::Mouse::Left)
						key = is.checkForAnyMouseInput().value();

					if (is.checkForAnyJoystickInput().has_value())
						key = is.checkForAnyJoystickInput().value();
				}

				if (keyBindChanger.button("Cancel##Change Keybindigs", ImVec2(100.f, 30.f)) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					keyBindChanger.close();
				ImGui::SameLine();
				if (keyBindChanger.button("OK##Change Keybindigs", ImVec2(100.f, 30.f)) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					toChange->second = key;
					if (!is.saveInput("res/inputSettings.ini"))
						ImGui::InsertNotification({ ImGuiToastType_Error, "Failed to save changes..." });
					else
						keyBindChanger.close();
				}
			}
			break;
		}
		case settingState::Graphics:
		{
			if (ImGui::Checkbox("Fullscreen: ", &settings::m_Fullscreen))
				window.create(settings::m_Videomodes[settings::m_currVideomode], "Fan Game!", settings::m_Fullscreen, settings::m_FpsLimit, settings::m_Vsync);

			ImGui::SetNextItemWidth(300.f);
			std::string preview = std::to_string(settings::m_Videomodes[settings::m_currVideomode].size.x) + "x" + std::to_string(settings::m_Videomodes[settings::m_currVideomode].size.y);
			if (ImGui::BeginCombo("Resolution: ", preview.c_str(), ImGuiComboFlags_HeightSmall))
			{
				for (std::size_t i = 0; i < settings::m_Videomodes.size(); ++i)
				{
					std::string text = std::to_string(settings::m_Videomodes[i].size.x) + "x" + std::to_string(settings::m_Videomodes[i].size.y);
					if (ImGui::Selectable(text.c_str()))
					{
						settings::m_currVideomode = (int)i;
						window.create(settings::m_Videomodes[settings::m_currVideomode], "Fan Game!", settings::m_Fullscreen, settings::m_FpsLimit, settings::m_Vsync);
						this->backgroundImage.setSize(window.getView().getSize());
					}
				}
				ImGui::EndCombo();
			}
			ImGui::Checkbox("Show FPS", &settings::m_ShowFPS);

			if (!settings::m_isFPSLimited)
				if (ImGui::Checkbox("VSync: ", &settings::m_Vsync))
					window.setVSync(settings::m_Vsync);

			if (!settings::m_Vsync)
			{
				if (ImGui::Checkbox("FPS Limit: ", &settings::m_isFPSLimited))
				{
					if (!settings::m_isFPSLimited)
						settings::m_FpsLimit = 0;
					window.setFramerateLimit(settings::m_FpsLimit);
				}
				if (ImGui::SliderInt("##Limit: ", &settings::m_FpsLimit, 30, 240))
					window.setFramerateLimit(settings::m_FpsLimit);
			}
			break;
		}
		case settingState::Game:
		{
			ImGui::SliderFloat("Vertical Sensivity: ", &settings::m_VerticalSensivity, 0.f, 20.f);
			ImGui::SliderFloat("Horizontal Sensivity: ", &settings::m_HorizontalSensivity, 0.f, 20.f);
			break;
		}
		case settingState::Mainmenu:
		{
			if (ImGui::SliderInt("Front image: ", &settings::m_currFrontPicture, 0, 0))
				this->frontImage.setTextureRect(sf::IntRect(sf::Vector2i(600 * settings::m_currFrontPicture, 0), sf::Vector2i(600, 600)));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				sf::Sprite background;
				background.setTexture(*this->frontImage.getTexture());
				background.setTextureRect(sf::IntRect(sf::Vector2i(600 * settings::m_currFrontPicture, 0),
					sf::Vector2i(600, 600)));
				ImGui::Image(background, ImVec2(300.f, 300.f));
				ImGui::EndTooltip();
			}

			if (ImGui::SliderInt("Profile picture: ", &settings::m_currProfilePicture, 0, 3))
				this->icon.setTextureRect(sf::IntRect(sf::Vector2i(100 * settings::m_currProfilePicture, 0),
					sf::Vector2i(100, 100)));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				sf::Sprite background;
				background.setTexture(*this->icon.getTexture());
				background.setTextureRect(sf::IntRect(sf::Vector2i(100 * settings::m_currProfilePicture, 0),
					sf::Vector2i(100, 100)));
				ImGui::Image(background, ImVec2(100.f, 100.f));
				ImGui::EndTooltip();
			}

			if (ImGui::SliderInt("Background image: ", &settings::m_currBackgroundPicture, 0, 3))
				this->backgroundImage.setTextureRect(sf::IntRect(sf::Vector2i(1920 * settings::m_currBackgroundPicture, 0),
					sf::Vector2i(1920, 1080)));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				sf::Sprite background;
				background.setTexture(*this->backgroundImage.getTexture());
				background.setTextureRect(sf::IntRect(sf::Vector2i(1920 * settings::m_currBackgroundPicture, 0),
					sf::Vector2i(1920, 1080)));
				ImGui::Image(background, ImVec2(300.f, 300.f));
				ImGui::EndTooltip();
			}
			break;
		}
		case settingState::Audio:
		{
			ImGui::Text("Currently playing: ");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(500.f);
			if (ImGui::BeginCombo("###MusicSelector", settings::m_currMusic.c_str(), ImGuiComboFlags_HeightSmall))
			{
				for (auto& music : settings::m_Music)
				{
					if (ImGui::Selectable(music))
					{
						if (settings::m_currMusic.c_str() != music)
						{
							settings::m_currMusic = music;
							this->m_MainMusic->stop();
							this->m_MainMusic = resourceSystem::get<std::unique_ptr<sf::Music>>(settings::m_currMusic).get();
							if (settings::m_MusicVolume > 0.f)
								this->m_MainMusic->play();
							this->m_MainMusic->setVolume(settings::m_MusicVolume);
						}
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SliderFloat("Game volume: ", &settings::m_GameVolume, 0.f, 100.f);
			if (ImGui::SliderFloat("Music volume: ", &settings::m_MusicVolume, 0.f, 100.f))
				this->m_MainMusic->setVolume(settings::m_MusicVolume);
			break;
		}
		default:
			break;
		}
		if (settingsPopUp.button("Back & Save##Settings", ImVec2(300.f, 75.f)))
		{
			if (settings::saveSettings("res/Settings.ini"))
				settingsPopUp.close();
		}
	}
}

const void menu::multiplayerPanel(window& window, const sf::Time& dt) noexcept
{
	return void();
}

const void menu::charactersPanel(window& window, const sf::Time& dt) noexcept
{
	popup CharactersPanel("Characters", window.getWindow(), this->characterPopUpShouldBeOpen);
	if (this->characterPopUpShouldBeOpen)
	{
		if (ImGui::BeginChild("##CharSet", ImVec2(600.f, 300.f), true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar))
		{
			for (std::size_t i = 0; i < this->characters.size(); ++i)
			{
				sf::Sprite temp;
				if (!this->characters[i].unlocked)
					temp.setTexture(resourceSystem::c_get<sf::Texture>("WallTexture"));
				else
					temp.setTexture(resourceSystem::c_get<sf::Texture>("CharacterTexture"));

				temp.setTextureRect(sf::IntRect(sf::Vector2i(64 * (int)i, 0), sf::Vector2i(64, 64)));
				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 25.f, ImGui::GetCursorPos().y));
				ImGui::Image(temp, sf::Vector2f(150.f, 150.f));

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					sf::Sprite ability;
					ability.setTexture(resourceSystem::c_get<sf::Texture>("WallTexture"));
					ability.setTextureRect(sf::IntRect(sf::Vector2i(128, 0), sf::Vector2i(64, 64)));
					ImGui::SetCursorPos(ImVec2(5.f, 5.f));
					ImGui::Image(ability, sf::Vector2f(50.f, 50.f));
					ImGui::SameLine();
					CharactersPanel.text("Can be an image of any ability or something like that.", ImVec4(1.f, 0.0f, 0.1f, 1.f));
					CharactersPanel.text("This will be updated later so it displays information about the charater that is currently being hoverd.", ImVec4(0.f, 0.8f, 0.1f, 1.f));
					ImGui::EndTooltip();
				}
				ImGui::SameLine();
				CharactersPanel.text("Some information about the character that is being displayed can go here.");

				if (!this->characters[i].unlocked)
				{
					ImGui::PushID("CharacterUnlock" + i);
					if (CharactersPanel.button("Unlock", ImVec2(200.f, 50.f)) && this->myAccount.currency > this->characters[i].price)
					{
						this->myAccount.currency -= this->characters[i].price;
						this->characters[i].unlocked = true;
					}
					ImGui::PopID();
					if (this->myAccount.currency < this->characters[i].price)
						CharactersPanel.tooltip("Not enough currency, come back later.\nAnd put some usefull information here later...", ImVec4(1.f, 0.f, 0.f, 1.f));
					else
						CharactersPanel.tooltip(reinterpret_cast<const char*>(u8"This item costs 690$."), ImVec4(0.f, 1.f, 0.4f, 1.f));
				}
			}
			ImGui::EndChild();
		}

		if (CharactersPanel.button("Open loot box", ImVec2(300.f, 75.f)) && this->myAccount.currency >= 420)
			this->myAccount.currency -= 420;

		if (this->myAccount.currency < 420)
			CharactersPanel.tooltip("Not enough currency, come back later.\nAnd put some usefull information here later...", ImVec4(1.f, 0.f, 0.f, 1.f));
		else
			CharactersPanel.tooltip("This item costs 420c.", ImVec4(0.f, 1.f, 0.4f, 1.f));

		if (ImGui::Button("Back##Characters", ImVec2(300.f, 75.f)))
			CharactersPanel.close();
	}
}

const bool menu::login(const std::string& name, const std::string& password) noexcept
{
	//TODO:
	//if login was succesful && remember me -> generate buffer so it will remember it next time...
	if (settings::rememberToStayLogedIn)
		std::printf("Do the magic...");

	request loginRequest("thefangamedb.000webhostapp.com", "/login.php");
	std::ostringstream stream = loginRequest.setParams<std::string>("username=", name, "&password=", password);

	if (!loginRequest.sendRequest(stream.str(), sf::seconds(3)))
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
	else
	{
		if (loginRequest.getResponse().getBody().find("Success.") == std::string::npos)
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Incorrect login details!"));
		else
		{
			std::stringstream ss(loginRequest.getResponse().getBody());
			std::string line;
			int x = 0;
			while (std::getline(ss, line, '#'))
			{
				if(x == 1)
					this->myAccount.account_lvl = std::stoi(line);
				if (x == 2)
					this->myAccount.xp = std::stof(line);
				if (x == 3)
					this->myAccount.xp_cap = std::stof(line);
				++x;
			}
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfully loged in!"));
			return true;
		}
	}
	return false;
}

const bool menu::createAccount(const std::string& name, const std::string& password, const std::string& email) noexcept
{
	if (email.find('@') == std::string::npos && email.find('.') == std::string::npos)
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Cannot create account, provided email is not valid!"));
	else
	{
		request createProfile("thefangamedb.000webhostapp.com", "/register.php");
		std::ostringstream stream = createProfile.setParams<std::string>("username=", name, "&password=", password, "&email=", email);

		if (!createProfile.sendRequest(stream.str(), sf::seconds(3)))
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
		else
		{
			if (createProfile.getResponse().getBody().find("Success.") == std::string::npos)
				ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Failed to create an account!"));
			else
			{
				ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfully created an account!"));
				return true;
			}
		}
	}
	return false;
}

const void menu::giveXP(const float& amount) noexcept
{
	this->myAccount.xp += amount;
	//LvL up...
	while (this->myAccount.xp >= this->myAccount.xp_cap)
	{
		this->myAccount.xp -= this->myAccount.xp_cap;
		this->myAccount.xp_cap += (10.f * this->myAccount.account_lvl);
		++this->myAccount.account_lvl;
	}
}