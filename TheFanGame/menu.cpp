#include "menu.h"

menu::menu(window& window) noexcept { this->m_window = &window; }

menu::~menu() noexcept 
{
	this->shutdownServer();
	ImGui::SFML::Shutdown(); 
	this->MainMusic.stop();
}

const void menu::init(sf::RenderWindow& window)
{
	ImGui::SFML::Init(window);
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("res/Gen Jyuu Gothic Monospace Bold.ttf", 25.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	ImGui::SFML::UpdateFontTexture();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.f;
	style.FrameRounding = 15.3f;
	style.ScrollbarRounding = 2.5f;
	style.GrabRounding = 2.5f;

	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);

	this->m_view.setSize(sf::Vector2f(window.getSize()));
	this->m_view.setCenter(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));

	//Load when he connect to the account...
	this->xp_bar.setPosition(sf::Vector2f(300.f, 85.f));
	this->xp_bar.setSize(sf::Vector2f(350.f, 15.f));
	this->xp_bar.setFillColor(sf::Color::White);
	this->xp_bar.setOutlineThickness(2.f);
	this->xp_bar.setOutlineColor(sf::Color::Black);

	this->curr_xp.setPosition(this->xp_bar.getPosition());
	this->curr_xp.setSize(sf::Vector2f(this->xp_bar.getSize().x * (this->myAccount.xp / this->myAccount.xp_cap), this->xp_bar.getSize().y));
	this->curr_xp.setFillColor(sf::Color::Yellow);
	this->curr_xp.setOutlineThickness(0.f);
	this->curr_xp.setOutlineColor(sf::Color::Yellow);
	//Load characters...
	for (std::size_t i = 0; i < 5; ++i)
		this->characters.emplace_back();

	if (this->loadSettings("res/Settings.ini"))
	{
		this->m_videomodes = sf::VideoMode::getFullscreenModes();

		this->backgroundImage.setTexture(&resourceManager::get<sf::Texture>("WallTexture"));
		this->backgroundImage.setSize(sf::Vector2f(window.getSize()));
		this->backgroundImage.setTextureRect(sf::IntRect(64 * this->currBackgroundPicture, 0, 64, 64));
		this->backgroundImage.setFillColor(sf::Color(255, 255, 255, 125));

		this->MainMusic.setBuffer(resourceManager::get<sf::SoundBuffer>("MainMusic"));
		this->MainMusic.setLoop(true);
		this->MainMusic.setVolume(this->music_volume);
		if (this->music_volume > 0.f)
			this->MainMusic.play();
	}
	else
		throw "Cannot load save file...\n";
}

const void menu::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	ImGui::SFML::Update(window, dt);

	if (this->music_volume == 0.f)
		this->MainMusic.stop();
	else if (this->music_volume > 0.f && this->MainMusic.getStatus() != sf::SoundSource::Playing)
		this->MainMusic.play();

	//sf::Vector2f scaleFactor = sf::Vector2f(static_cast<float>(window.getSize().x) / static_cast<float>(this->m_videomodes[0].width),
	//										  static_cast<float>(window.getSize().y) / static_cast<float>(this->m_videomodes[0].height));

	if (ImGui::Begin("Main Window", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::SetWindowSize("Main Window", ImVec2((float)window.getSize().x, (float)window.getSize().y));
		ImGui::SetWindowPos("Main Window", ImVec2(0.f, 0.f));

		ImVec2 WidgetPos;
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		if (this->m_ShowFPS)
		{
			ImGui::SetCursorPos(ImVec2(vMax.x - 150.f, vMin.y));
			ImGui::Text("FPS: %.3f", 1.f / dt.asSeconds());
		}

		if (this->m_ServerError)
		{
			if (ImGui::Begin("Error!", &this->m_ServerError, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowFocus("Error!");
				ImGui::SetWindowSize("Error!", ImVec2(400.f, 100.f));
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "An error has occured!\nCannot connect to the servers.\n");
				ImGui::End();
			}
		}
		
		switch (this->m_State)
		{
		case state::MainMenu:
			ImGui::SetCursorPos(ImVec2(vMax.x - 700.f, vMin.y + 40.f));
			ImGui::Text("Currently playing: ");
			ImGui::SameLine();
			WidgetPos = ImGui::GetCursorPos();
			ImGui::SetNextItemWidth(500.f);
			ImGui::SetCursorPos(ImVec2(WidgetPos.x - 15.f, WidgetPos.y));
			if (ImGui::BeginCombo("###MusicSelector", "Music Name", ImGuiComboFlags_HeightSmall))
			{
				for (std::size_t i = 0; i < 10; ++i)
				{
					ImGui::PushID("MusicName" + i);
					ImGui::Selectable("||||");
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}

			this->frontPicture.setTexture(resourceManager::get<sf::Texture>("WallTexture"));
			this->frontPicture.setTextureRect(sf::IntRect(64 * this->currFrontPicture, 0, 64, 64));
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x + 600.f, ImGui::GetWindowContentRegionMin().y + 100.f));
			ImGui::Image(this->frontPicture, sf::Vector2f(ImGui::GetWindowContentRegionMax().x / 1.6f, ImGui::GetWindowContentRegionMax().y / 1.3f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("This is the front image!");

			this->profilePicture.setTexture(resourceManager::get<sf::Texture>("WallTexture"));
			this->profilePicture.setTextureRect(sf::IntRect(64 * this->currProfilePicture, 0, 64, 64));
			ImGui::SetCursorPos(ImVec2(vMin.x + 25.f, vMin.y + 5.f));
			ImGui::Image(this->profilePicture, sf::Vector2f(100.f, 100.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("This is the profile picture!");

			ImGui::SameLine();
			ImGui::Text("Account: %s\nXP: %0.f / %0.f\nLevel: %i\nCoverCoin: %i$",
				this->myAccount.account_name,
				this->myAccount.xp, this->myAccount.xp_cap,
				this->myAccount.account_lvl,
				this->myAccount.currency);

			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 75.f));
			WidgetPos = ImGui::GetCursorPos();
			if (ImGui::Button("Play", ImVec2(300.f, 75.f)))
				this->m_PlaySelected ? this->m_PlaySelected = false : this->m_PlaySelected = true;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Start playing the game RIGHT NOW!");

			ImGui::SetCursorPos(ImVec2(WidgetPos.x, WidgetPos.y + 125.f));
			if (ImGui::Button("Characters", ImVec2(300.f, 75.f)))
				this->m_State = state::Characters;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Unlocked characters and the character shop, etc...");

			ImGui::SetCursorPos(ImVec2(WidgetPos.x, WidgetPos.y + 250.f));
			if (ImGui::Button("Settings", ImVec2(300.f, 75.f)))
				this->m_State = state::Settings;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Opens the setting menu.");

			ImGui::SetCursorPos(ImVec2(WidgetPos.x, WidgetPos.y + 375.f));
			if (ImGui::Button("Quit", ImVec2(300.f, 75.f)))
				window.close();
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Goodbye!");

			if (this->m_PlaySelected)
			{
				ImGui::SetCursorPos(ImVec2(WidgetPos.x + 305.f, WidgetPos.y - 15.f));
				if (ImGui::Button("Singleplayer", ImVec2(200.f, 50.f)))
				{
					this->m_PlaySelected = false;
					this->m_State = state::Singleplayer;
					this->MainMusic.stop();
					this->m_window->addState<game>();
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Starts the game state.");

				ImGui::SetCursorPos(ImVec2(WidgetPos.x + 305.f, WidgetPos.y + 45.f));
				if (ImGui::Button("Multiplayer", ImVec2(200.f, 50.f)))
				{
					if (this->socket.connect(this->serverIP, this->serverPort, sf::seconds(3.f)) != sf::Socket::Done)
					{
						this->m_PlaySelected = false;
						this->m_ServerError = true;
					}
					else
					{
						this->m_PlaySelected = false;
						this->refreshServerList();
						this->m_State = state::Multiplayer;
					}
				}
			}
			break;
		case state::Settings:
			if (ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
			{
				ImGui::SetWindowSize("Settings", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Settings", ImVec2(25.f, 25.f));

				vMin = ImGui::GetWindowContentRegionMin();
				vMax = ImGui::GetWindowContentRegionMax();
				vMin.x += ImGui::GetWindowPos().x;
				vMin.y += ImGui::GetWindowPos().y;
				vMax.x += ImGui::GetWindowPos().x;
				vMax.y += ImGui::GetWindowPos().y;

				if (ImGui::BeginMenuBar())
				{
					if (ImGui::MenuItem("Mainmenu"))
						this->m_SettingState = settingState::Mainmenu;
					if (ImGui::MenuItem("Game"))
						this->m_SettingState = settingState::Game;
					if (ImGui::MenuItem("Graphics"))
						this->m_SettingState = settingState::Graphics;
					if (ImGui::MenuItem("Input"))
						this->m_SettingState = settingState::Input;
					if (ImGui::MenuItem("Audio"))
						this->m_SettingState = settingState::Audio;
					ImGui::EndMenuBar();
				}
				switch (this->m_SettingState)
				{
				case settingState::Input:
					for (auto& it : inputManager::m_Action)
					{
						ImGui::Text(it.first);
						ImGui::SameLine();
						if (ImGui::Button(inputManager::convert(it.second), ImVec2(300.f, 30.f)))
							std::cout << "Ok\n";
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200.f);
						ImGui::PushID(it.first + static_cast<std::uint32_t>(it.second.m_InputType));
						if (ImGui::BeginCombo("###PressOrRelease", "Press", ImGuiComboFlags_HeightSmall))
						{
							ImGui::EndCombo();
						}
						ImGui::PopID();
					}
					break;
				case settingState::Graphics:
					if (ImGui::Checkbox("Fullscreen: ", &this->fullscreen))
						this->m_window->setFullscreen(this->fullscreen);

					if (ImGui::BeginListBox("Resolution: ", ImVec2(200, 200)))
					{
						for (std::size_t i = 0; i < this->m_videomodes.size(); ++i)
						{
							std::string text = std::to_string(this->m_videomodes[i].width) + "x" + std::to_string(this->m_videomodes[i].height);
							if (ImGui::Selectable(text.c_str()))
							{
								this->m_window->setSize(sf::Vector2u(this->m_videomodes[i].width, this->m_videomodes[i].height));
								this->backgroundImage.setSize(sf::Vector2f(window.getSize()));
								this->m_view.setSize(sf::Vector2f(window.getSize()));
								this->m_view.setCenter(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));
							}
						}
						ImGui::EndListBox();
					}
					if (ImGui::Checkbox("Show FPS", &this->m_ShowFPS))
						std::cout << "FPS stuff...\n";
					if (ImGui::Checkbox("FPS Limit: ", &this->isFPSLimited))
					{
						if (!this->isFPSLimited)
							this->m_window->setFramerateLimit(0);
						else
							this->m_window->setFramerateLimit(this->fps_limit);
					}
					if (this->isFPSLimited)
					{
						ImGui::SameLine();
						if (ImGui::SliderInt("##Limit: ", &this->fps_limit, 30, 180))
							this->m_window->setFramerateLimit(this->fps_limit);
					}
					break;
				case settingState::Game:
					ImGui::SliderFloat("Sensivity: ", &this->sensivity, 0.f, 20.f);
					break;
				case settingState::Mainmenu:
					if (ImGui::InputText("Account name: ", this->myAccount.account_name, MAX_CHAR_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
						std::cout << "New account name: " << this->myAccount.account_name << '\n';
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Set your account name here.\nYou can just type it and it will be automaticaly saved.\nMaximum characters that are allowed is 16.");

					ImGui::SliderInt("Front image: ", &this->currFrontPicture, 0, 5);
					ImGui::SliderInt("Profile picture: ", &this->currProfilePicture, 0, 5);
					if (ImGui::SliderInt("Background image: ", &this->currBackgroundPicture, 0, 5))
						this->backgroundImage.setTextureRect(sf::IntRect(64 * this->currBackgroundPicture, 0, 64, 64));
					break;
				case settingState::Audio:
					ImGui::SliderFloat("Game volume: ", &this->game_volume, 0.f, 100.f);
					if (ImGui::SliderFloat("Music volume: ", &this->music_volume, 0.f, 100.f))
						this->MainMusic.setVolume(this->music_volume);
					break;
				default:
					break;
				}
				ImGui::SetCursorPos(ImVec2(vMax.x - 350.f, vMax.y - 125.f));
				if (ImGui::Button("Back##Settings", ImVec2(300.f, 75.f)))
				{
					if (this->saveSettings("res/Settings.ini"))
						this->m_State = state::MainMenu;
				}
			}ImGui::End();
			break;
		case state::Characters:
			if (ImGui::Begin("Characters", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Characters", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Characters", ImVec2(25.f, 25.f));

				vMin = ImGui::GetWindowContentRegionMin();
				vMax = ImGui::GetWindowContentRegionMax();
				vMin.x += ImGui::GetWindowPos().x;
				vMin.y += ImGui::GetWindowPos().y;
				vMax.x += ImGui::GetWindowPos().x;
				vMax.y += ImGui::GetWindowPos().y;

				if (ImGui::BeginChild("##CharSet", ImVec2(0, ImGui::GetWindowContentRegionMax().y - 200.f), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar))
				{
					int y = 0;
					int x = 0;
					for (std::size_t i = 0; i < this->characters.size(); ++i)
					{
						if (i != 0 && i % 2 == 0)
						{
							++x;
							y = 0;
						}

						sf::Sprite temp;
						if (!this->characters[i].unlocked)
							temp.setTexture(resourceManager::get<sf::Texture>("WallTexture"));
						else
							temp.setTexture(resourceManager::get<sf::Texture>("CharacterTexture"));

						temp.setTextureRect(sf::IntRect(64 * (int)i, 0, 64, 64));
						ImGui::SetCursorPos(ImVec2(100.f + 350.f * (float)x, 100.f + 300.f * (float)y));
						WidgetPos = ImGui::GetCursorPos();
						ImGui::Image(temp, sf::Vector2f(150.f, 150.f));

						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							sf::Sprite ability;
							ability.setTexture(resourceManager::get<sf::Texture>("WallTexture"));
							ability.setTextureRect(sf::IntRect(128, 0, 64, 64));
							ImGui::SetCursorPos(ImVec2(5.f, 5.f));
							ImGui::Image(ability, sf::Vector2f(50.f, 50.f));
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1.f, 0.0f, 0.1f, 1.f), "Can be an image of any ability or something like that.");
							ImGui::TextColored(ImVec4(0.f, 0.8f, 0.1f, 1.f), "This will be updated later so it displays information\nabout the charater that is currently being hoverd.");
							ImGui::EndTooltip();
						}
						ImGui::SameLine();
						ImGui::Text("Some information\nabout the\ncharacter that is\nbeing displayed\ncan go here.");

						if (!this->characters[i].unlocked)
						{
							ImGui::PushID("CharacterUnlock" + i);
							ImGui::SetCursorPos(ImVec2(WidgetPos.x - 25.f, WidgetPos.y + 175.f));
							if (ImGui::Button("Unlock", ImVec2(200.f, 50.f)))
							{
								if (this->myAccount.currency > this->characters[i].price)
								{
									this->myAccount.currency -= this->characters[i].price;
									this->characters[i].unlocked = true;
								}
							}
							if (ImGui::IsItemHovered())
							{
								if (this->myAccount.currency < this->characters[i].price)
								{
									ImGui::BeginTooltip();
									ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Not enough currency, come back later.\nAnd put some usefull information here later...");
									ImGui::EndTooltip();
								}
								else
								{
									ImGui::BeginTooltip();
									ImGui::TextColored(ImVec4(0.f, 1.f, 0.4f, 1.f), reinterpret_cast<const char*>(u8"This item costs 690$."));
									ImGui::EndTooltip();
								}
							}
							ImGui::PopID();
						}
						++y;
					}
				}ImGui::EndChild();

				ImGui::SetCursorPos(ImVec2(vMin.x, vMax.y - 125.f));
				if (ImGui::Button("Open loot box", ImVec2(300.f, 75.f)))
				{
					if (this->myAccount.currency >= 420)
					{
						this->myAccount.currency -= 420;
						std::cout << "Will do it later... But this will cost lem_SettingState than stuff above...\n";
					}
				}
				if (ImGui::IsItemHovered())
				{
					if (this->myAccount.currency < 420)
					{
						ImGui::BeginTooltip();
						ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Not enough currency, come back later.\nAnd put some usefull information here later...");
						ImGui::EndTooltip();
					}
					else
					{
						ImGui::BeginTooltip();
						ImGui::TextColored(ImVec4(0.f, 1.f, 0.4f, 1.f), "This item costs 420c.");
						ImGui::EndTooltip();
					}
				}

				ImGui::SetCursorPos(ImVec2(vMax.x - 350.f, vMax.y - 125.f));
				if (ImGui::Button("Back##Characters", ImVec2(300.f, 75.f)))
					this->m_State = state::MainMenu;
			}ImGui::End();
			break;
		case state::Multiplayer:
			if (ImGui::Begin("Multiplayer", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Multiplayer", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Multiplayer", ImVec2(25.f, 25.f));

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
						ImGui::PushID("ServerInfo" + i);
						if (ImGui::Selectable(serverInfo.c_str()))
						{
							std::cout << "connecting to the server... " << this->servers[i].first.toString() << ":" << this->servers[i].second << "\n";
							this->socket.disconnect();
							if (this->socket.connect(this->servers[i].first, this->servers[i].second) == sf::Socket::Done)
							{
								std::cout << "Succesfuly connected to the server...\n";
								//Get lobby data from server
								//Load them
								this->m_State = state::MultiLobby;
							}
						}
						ImGui::PopID();
					}
					ImGui::EndListBox();
				}

				if (ImGui::Button("Refress"))
					this->refreshServerList();

				ImGui::SetCursorPos(ImVec2(vMin.x, vMax.y - 175.f));
				ImGui::Text("Server IP: ");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(window.getSize().x / 5.f);
				if (ImGui::InputTextWithHint("##ServerIP", "127.0.0.0", this->InputIp, 12, ImGuiInputTextFlags_EnterReturnsTrue))
					std::cout << this->InputIp << "\n";
				ImGui::SameLine();
				ImGui::Text("Port: ");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(window.getSize().x / 8.f);
				if (ImGui::InputTextWithHint("##Port", "52420", this->InputPort, 6, ImGuiInputTextFlags_EnterReturnsTrue))
					std::cout << this->InputPort << "\n";
				ImGui::SameLine();
				WidgetPos = ImGui::GetCursorPos();
				ImGui::SetCursorPos(ImVec2(WidgetPos.x, WidgetPos.y - 10.f));
				if (ImGui::Button("Join", ImVec2(200.f, 50.f)))
				{
					//this->socket.disconnect();
					//this->socket.connect();
					//if succesful 
					// -> get data from lobbya
					// -> load lobby
				}

				ImGui::SetCursorPos(ImVec2(vMin.x + 25.f, vMax.y - 125.f));
				if (ImGui::Button("Host", ImVec2(300.f, 75.f)))
				{
					if (this->serverThread == nullptr)
					{
						std::cout << "Started hosting a server...\n";
						this->serverThread = std::make_unique<sf::Thread>(&menu::startServer, this);
						this->serverThread->launch();
					}
					else
						std::cout << "You are already hosting a game...\n";
				}

				ImGui::SetCursorPos(ImVec2(vMax.x - 350.f, vMax.y - 125.f));
				if (ImGui::Button("Back##Multiplayer", ImVec2(300.f, 75.f)))
				{
					this->shutdownServer();
					this->servers.clear();
					this->socket.disconnect();
					this->m_State = state::MainMenu;
				}
			}ImGui::End();
			break;
		case state::Singleplayer:
			if (ImGui::Begin("Singleplayer", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Singleplayer", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Singleplayer", ImVec2(25.f, 25.f));
				ImGui::End();
			}
			break;
		case state::MultiLobby:
			if (ImGui::Begin("Lobby", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Lobby", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Lobby", ImVec2(25.f, 25.f));

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
					for (std::size_t i = 0; i < 12; ++i)
					{
						ImGui::PushID("Player" + i);
						if (ImGui::Selectable("Player"))
						{
							//Mute
							// if admin...
							//	Kick
							//	Ban
						}
						ImGui::PopID();
					}
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
					}ImGui::EndListBox();
					//Switch case maybie??? idk...
					ImGui::Text("GameModeTextGoesHere");
					if (ImGui::BeginListBox("Map maybie?"))
					{
						for (std::size_t i = 0; i < 5; ++i)
						{
							ImGui::PushID("Maps" + i);
							if (ImGui::Selectable("Stuff"))
								std::cout << "Ok!\n";
							ImGui::PopID();
						}
					}ImGui::EndListBox();
					//Switch case maybie??? idk...
					ImGui::Text("MapTextGoesHere");
				}ImGui::EndChild();

				ImGui::SetCursorPos(ImVec2(vMin.x + 25.f, vMax.y - 125.f));
				if (ImGui::Button("Start##Lobby", ImVec2(300.f, 75.f)))
					std::cout << "Staring lobby...\n";

				ImGui::SetCursorPos(ImVec2(vMax.x - 350.f, vMax.y - 125.f));
				if (ImGui::Button("Back##MultiLobby", ImVec2(300.f, 75.f)))
				{
					this->shutdownServer();
					this->refreshServerList();
					this->m_State = state::Multiplayer;
				}
			}ImGui::End();
			break;
		default:
			ImGui::Text("You're not suposed to see this LOL...\nSomething went horibly wrong...");
			ImGui::LogButtons();
			if (ImGui::Button("Help me!"))
				this->m_State = state::MainMenu;
			break;
		}
		ImGui::End();
	}
}

const void menu::processEvent(const sf::Event& event) noexcept 
{ 
	ImGui::SFML::ProcessEvent(event);

	if (event.type == sf::Event::Resized)
	{
		this->backgroundImage.setSize(sf::Vector2f((float)event.size.width, (float)event.size.height));
		this->m_view.setSize(sf::Vector2f((float)event.size.width, (float)event.size.height));
		this->m_view.setCenter(sf::Vector2f((float)event.size.width / 2.f, (float)event.size.height / 2.f));
	}
}

const void menu::draw(sf::RenderWindow& window) noexcept 
{
	window.setView(this->m_view);
	window.draw(this->backgroundImage);
	window.draw(this->xp_bar);
	window.draw(this->curr_xp);
	window.setView(window.getDefaultView());
	ImGui::SFML::Render(window);
}

const bool menu::saveSettings(const std::string& filePath) const noexcept
{
	std::ofstream saveFile;
	saveFile.open(filePath, std::ios::out | std::ios::trunc);
	if (saveFile.is_open())
	{
		saveFile << this->music_volume << '\n';
	}
	else
		return false;
	saveFile.close();
	return true;
}

const bool menu::loadSettings(const std::string& filePath) noexcept
{
	std::ifstream loadFile;
	loadFile.open(filePath, std::ios::in);
	if (loadFile.is_open())
	{
		loadFile >> this->music_volume;
	}
	else
		return false;
	loadFile.close();
	return true;
}

void menu::startServer()
{
	if (this->hosting.listen(sf::TcpListener::AnyPort, sf::IpAddress::getLocalAddress()) == sf::Socket::Done)
	{
		this->m_State = state::MultiLobby;

		sf::Packet packet;
		packet << sf::IpAddress::getLocalAddress().toString() << this->hosting.getLocalPort();
		if (this->socket.send(packet) != sf::Socket::Done)
			return;
		packet.clear();

		this->selector.add(this->hosting);

		while (this->m_window)
		{
			if (this->selector.wait())
			{
				if (this->selector.isReady(this->hosting))
				{
					sf::TcpSocket* client = new sf::TcpSocket;
					if (this->hosting.accept(*client) == sf::Socket::Done)
					{
						this->clients.push_back(client);
						this->selector.add(*client);
						std::cout << "A client has connected: " << client->getRemoteAddress() << ":" << client->getRemotePort() << '\n';
					}
					else
					{
						delete client;
					}
				}
				else
				{
					for (std::vector<sf::TcpSocket*>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
					{
						sf::TcpSocket& client = **it;
						if (this->selector.isReady(client))
						{
							sf::Packet packet;
							if (client.receive(packet) == sf::Socket::Disconnected)
							{
								this->selector.remove(client);
								client.disconnect();
								this->clients.erase(it);
								break;
							}
							if (client.receive(packet) == sf::Socket::Done)
							{
								char msg[255] = { "" };
								packet >> msg;
								std::cout << "Client " << client.getRemoteAddress() << " sent us a msg: " << msg << '\n';
							}
							packet.clear();
						}
					}
				}
			}
		}
	}
	else
		std::cout << "Error cannot start server...\n";
}

void menu::shutdownServer()
{
	if (this->serverThread != nullptr)
	{
		this->serverThread->terminate();
		this->serverThread.release();
		this->selector.clear();
		this->hosting.close();

		sf::Packet packet;
		packet << std::uint8_t(52);
		if (this->socket.send(packet) != sf::Socket::Done)
			std::cout << "Error! Cannot send quit msg to the server...\n";
		packet.clear();
	}
}

void menu::refreshServerList()
{
	this->servers.clear();

	sf::Packet packet;
	packet << std::uint8_t(53);
	if (this->socket.send(packet) != sf::Socket::Done)
		std::cout << "Cannot refress server list...\n";
	packet.clear();

	if (this->socket.receive(packet) == sf::Socket::Done)
	{
		packet >> this->activeServerNum;
		packet.clear();
		for (std::size_t i = 0; i < this->activeServerNum; ++i)
		{
			if (this->socket.receive(packet) == sf::Socket::Done)
			{
				std::string ip;
				sf::Uint16 g_port;
				packet >> ip >> g_port;
				this->servers.push_back(std::pair<sf::IpAddress, sf::Uint16>(sf::IpAddress(ip), g_port));
				std::cout << this->servers[i].first.toString() << ":" << this->servers[i].second << '\n';
			}
			packet.clear();
		}
	}
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
	this->curr_xp.setSize(sf::Vector2f(this->xp_bar.getSize().x * (this->myAccount.xp / this->myAccount.xp_cap), this->xp_bar.getSize().y));
}