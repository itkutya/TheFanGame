#include "menu.h"

menu::menu(window& window) noexcept { this->m_window = &window; }

menu::~menu() noexcept 
{
	if (!this->shutdownServer())
	{
		if (this->handleLocalPlayerNum != nullptr)
		{
			this->handleLocalPlayerNum->terminate();
			this->handleLocalPlayerNum.release();
		}
	}
	this->m_MainMusic->stop();
	ImGui::SFML::Shutdown();
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
	style.FrameRounding = 20.5f;
	style.ScrollbarRounding = 20.5f;
	style.GrabRounding = 20.5f;

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.44f, 0.44f, 0.60f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.57f, 0.57f, 0.57f, 0.70f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.80f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.36f, 0.36f, 0.36f, 0.54f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

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

	this->backgroundImage.setSize(sf::Vector2f(window.getSize()));
	this->m_view.setSize(sf::Vector2f(window.getSize()));
	this->m_view.setCenter(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));

	this->frontImage.setTexture(resourceSystem::get_c<sf::Texture>("FrontImage"));
	this->frontImage.setTextureRect(sf::IntRect(600 * settings::m_currFrontPicture, 0, 600, 600));

	this->icon.setTexture(resourceSystem::get_c<sf::Texture>("Icon"));
	this->icon.setTextureRect(sf::IntRect(100 * settings::m_currProfilePicture, 0, 100, 100));

	this->backgroundImage.setTexture(&resourceSystem::get_c<sf::Texture>("Background"));
	this->backgroundImage.setSize(sf::Vector2f(window.getSize()));
	this->backgroundImage.setTextureRect(sf::IntRect(1920 * settings::m_currBackgroundPicture, 0, 1920, 1080));
	this->backgroundImage.setFillColor(sf::Color(255, 255, 255, 200));

	this->m_MainMusic = &resourceSystem::get<sf::MyMusic>(settings::m_currMusic);
	this->m_MainMusic->setLoop(true);
	if (settings::m_MusicVolume > 0.f)
		this->m_MainMusic->play();
	this->m_MainMusic->setVolume(settings::m_MusicVolume);
}

const void menu::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	ImGui::SFML::Update(window, dt);

	if (settings::m_MusicVolume == 0.f)
		this->m_MainMusic->pause();
	else if (settings::m_MusicVolume > 0.f && this->m_MainMusic->getStatus() == sf::SoundSource::Stopped)
		this->m_MainMusic->play();

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

		std::string preview = std::to_string(settings::m_Videomodes[settings::m_currVideomode].width) + "x" + std::to_string(settings::m_Videomodes[settings::m_currVideomode].height);

		if (settings::m_ShowFPS)
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
			ImGui::SetCursorPos(ImVec2(vMax.x - 750.f, vMin.y + 40.f));
			if (this->m_MainMusic->getStatus() == sf::SoundSource::Playing)
			{
				if (ImGui::ImageButton(resourceSystem::get_c<sf::Texture>("Pause"), sf::Vector2f(35.f, 35.f)))
					this->m_MainMusic->pause();
			}
			else
				if (ImGui::ImageButton(resourceSystem::get_c<sf::Texture>("Resume"), sf::Vector2f(35.f, 35.f)))
					this->m_MainMusic->play();
			ImGui::SameLine();
			WidgetPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(WidgetPos.x, WidgetPos.y + 5.f));
			ImGui::Text("Currently playing: ");
			ImGui::SameLine();
			WidgetPos = ImGui::GetCursorPos();
			ImGui::SetNextItemWidth(500.f);
			ImGui::SetCursorPos(ImVec2(WidgetPos.x - 15.f, WidgetPos.y + 5.f));
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
							this->m_MainMusic = &resourceSystem::get<sf::MyMusic>(settings::m_currMusic);
							if (settings::m_MusicVolume > 0.f)
								this->m_MainMusic->play();
							this->m_MainMusic->setVolume(settings::m_MusicVolume);
						}
					}
				}
				ImGui::EndCombo();
			}

			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMin().x + 600.f, ImGui::GetWindowContentRegionMin().y + 100.f));
			ImGui::Image(this->frontImage, sf::Vector2f(ImGui::GetWindowContentRegionMax().x / 1.6f, ImGui::GetWindowContentRegionMax().y / 1.3f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("This is the front image!");

			ImGui::SetCursorPos(ImVec2(vMin.x + 25.f, vMin.y + 5.f));
			ImGui::Image(this->icon, sf::Vector2f(100.f, 100.f));
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
					this->m_MainMusic->stop();
					stateSystem::add<game>(*m_window);
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
				ImGui::SetWindowPos("Settings", ImVec2(50.f, 50.f));

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
					if (ImGui::MenuItem("Profile"))
						this->m_SettingState = settingState::Profile;
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
				case settingState::Profile:
					if (ImGui::InputText("Account name: ", this->myAccount.account_name, MAX_CHAR_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
						std::cout << "New account name: " << this->myAccount.account_name << '\n';
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Set your account name here.\nYou can just type it and it will be automaticaly saved.\nMaximum characters that are allowed is 16.");
					break;
				case settingState::Input:
					for (auto& it : inputSystem::m_Action)
					{
						ImGui::Text(it.first.c_str());
						ImGui::SameLine();
						if (ImGui::Button(inputSystem::convert(it.second), ImVec2(300.f, 30.f)))
							std::cout << "Ok\n";
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200.f);
						ImGui::PushID(it.first.c_str() + static_cast<std::uint32_t>(it.second.m_InputType));
						if (ImGui::BeginCombo("###PressOrRelease", inputSystem::PressOrRelease(it.second), ImGuiComboFlags_HeightSmall))
						{
							if (ImGui::Selectable("Press"))
								std::cout << "Later...\n";
							if (ImGui::Selectable("Release"))
								std::cout << "Later...\n";
							ImGui::EndCombo();
						}
						ImGui::PopID();
					}
					break;
				case settingState::Graphics:
					if (ImGui::Checkbox("Fullscreen: ", &settings::m_Fullscreen))
						this->m_window->recreate();

					ImGui::SetNextItemWidth(300.f);
					if (ImGui::BeginCombo("Resolution: ", preview.c_str(), ImGuiComboFlags_HeightSmall))
					{
						for (std::size_t i = 0; i < settings::m_Videomodes.size(); ++i)
						{
							std::string text = std::to_string(settings::m_Videomodes[i].width) + "x" + std::to_string(settings::m_Videomodes[i].height);
							if (ImGui::Selectable(text.c_str()))
							{
								settings::m_currVideomode = (int)i;
								this->m_window->recreate();
								this->backgroundImage.setSize(sf::Vector2f(window.getSize()));
								this->m_view.setSize(sf::Vector2f(window.getSize()));
								this->m_view.setCenter(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));
							}
						}
						ImGui::EndCombo();
					}
					if (ImGui::Checkbox("Show FPS", &settings::m_ShowFPS))
						std::cout << "\n";

					if (!settings::m_isFPSLimited)
						if (ImGui::Checkbox("VSync: ", &settings::m_Vsync))
							this->m_window->setVSync();

					if (!settings::m_Vsync)
					{
						if (ImGui::Checkbox("FPS Limit: ", &settings::m_isFPSLimited))
							this->m_window->setFramerateLimit();
						if (settings::m_isFPSLimited)
						{
							ImGui::SameLine();
							if (ImGui::SliderInt("##Limit: ", &settings::m_FpsLimit, 30, 240))
								this->m_window->setFramerateLimit();
						}
					}
					break;
				case settingState::Game:
					ImGui::SliderFloat("Vertical Sensivity: ", &settings::m_VerticalSensivity, 0.f, 20.f);
					ImGui::SliderFloat("Horizontal Sensivity: ", &settings::m_HorizontalSensivity, 0.f, 20.f);
					break;
				case settingState::Mainmenu:
					if(ImGui::SliderInt("Front image: ", &settings::m_currFrontPicture, 0, 0))
						this->frontImage.setTextureRect(sf::IntRect(600 * settings::m_currFrontPicture, 0, 600, 600));
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						sf::Sprite background;
						background.setTexture(*this->frontImage.getTexture());
						background.setTextureRect(sf::IntRect(600 * settings::m_currFrontPicture, 0, 600, 600));
						ImGui::Image(background, ImVec2(300.f, 300.f));
						ImGui::EndTooltip();
					}

					if(ImGui::SliderInt("Profile picture: ", &settings::m_currProfilePicture, 0, 3))
						this->icon.setTextureRect(sf::IntRect(100 * settings::m_currProfilePicture, 0, 100, 100));
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						sf::Sprite background;
						background.setTexture(*this->icon.getTexture());
						background.setTextureRect(sf::IntRect(100 * settings::m_currProfilePicture, 0, 100, 100));
						ImGui::Image(background, ImVec2(100.f, 100.f));
						ImGui::EndTooltip();
					}

					if (ImGui::SliderInt("Background image: ", &settings::m_currBackgroundPicture, 0, 3))
						this->backgroundImage.setTextureRect(sf::IntRect(1920 * settings::m_currBackgroundPicture, 0, 1920, 1080));
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						sf::Sprite background;
						background.setTexture(*this->backgroundImage.getTexture());
						background.setTextureRect(sf::IntRect(1920 * settings::m_currBackgroundPicture, 0, 1920, 1080));
						ImGui::Image(background, ImVec2(300.f, 300.f));
						ImGui::EndTooltip();
					}
					break;
				case settingState::Audio:
					ImGui::Text("Currently playing: ");
					ImGui::SameLine();
					WidgetPos = ImGui::GetCursorPos();
					ImGui::SetNextItemWidth(500.f);
					ImGui::SetCursorPos(ImVec2(WidgetPos.x - 15.f, WidgetPos.y));
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
									this->m_MainMusic = &resourceSystem::get<sf::MyMusic>(settings::m_currMusic);
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
				default:
					break;
				}
				ImGui::SetCursorPos(ImVec2(vMax.x - 750.f, vMax.y - 125.f));
				if (ImGui::Button("Save##Settings", ImVec2(300.f, 75.f)))
				{
					if (settings::saveSettings("res/Settings.ini"))
						std::cout << "Saved the settings...\n";
				}
				ImGui::SetCursorPos(ImVec2(vMax.x - 350.f, vMax.y - 125.f));
				if (ImGui::Button("Back##Settings", ImVec2(300.f, 75.f)))
				{
					if (settings::saveSettings("res/Settings.ini"))
						this->m_State = state::MainMenu;
				}
			}ImGui::End();
			break;
		case state::Characters:
			if (ImGui::Begin("Characters", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Characters", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Characters", ImVec2(50.f, 50.f));

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
							temp.setTexture(resourceSystem::get_c<sf::Texture>("WallTexture"));
						else
							temp.setTexture(resourceSystem::get_c<sf::Texture>("CharacterTexture"));

						temp.setTextureRect(sf::IntRect(64 * (int)i, 0, 64, 64));
						ImGui::SetCursorPos(ImVec2(100.f + 350.f * (float)x, 100.f + 300.f * (float)y));
						WidgetPos = ImGui::GetCursorPos();
						ImGui::Image(temp, sf::Vector2f(150.f, 150.f));

						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							sf::Sprite ability;
							ability.setTexture(resourceSystem::get_c<sf::Texture>("WallTexture"));
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
				ImGui::SetWindowPos("Multiplayer", ImVec2(50.f, 50.f));

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
						}
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
					std::cout << "connecting to the server... " << this->InputIp << ":" << this->InputPort << "\n";
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
				ImGui::SetWindowPos("Singleplayer", ImVec2(50.f, 50.f));
				ImGui::End();
			}
			break;
		case state::MultiLobby:
			if (ImGui::Begin("Lobby", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Lobby", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Lobby", ImVec2(50.f, 50.f));

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

void menu::startServer()
{
	if (this->hosting.listen(sf::TcpListener::AnyPort, sf::IpAddress::getLocalAddress()) == sf::Socket::Done)
	{
		this->m_State = state::MultiLobby;
		this->localHostPlayers.push_back(std::pair<sf::IpAddress, sf::Uint16>(sf::IpAddress(sf::IpAddress().getLocalAddress()), this->hosting.getLocalPort()));

		sf::Packet packet;
		packet << sf::IpAddress::getLocalAddress().toString() << this->hosting.getLocalPort();
		if (this->socket.send(packet) != sf::Socket::Done)
			return;
		packet.clear();

		this->selector.add(this->hosting);

		while (this->m_window)
		{
			if (this->selector.wait(sf::seconds(1.f)))
			{
				if (this->selector.isReady(this->hosting))
				{
					sf::TcpSocket* client = new sf::TcpSocket;
					if (this->hosting.accept(*client) == sf::Socket::Done)
					{
						this->clients.push_back(client);
						this->selector.add(*client);
						std::cout << "A client has connected: " << client->getRemoteAddress() << ":" << client->getRemotePort() << '\n';

						sf::Lock lock(this->mutex);
						this->localHostPlayers.push_back(std::pair<sf::IpAddress, sf::Uint16>(sf::IpAddress(client->getRemoteAddress()), client->getRemotePort()));
						for (std::size_t i = 0; i < this->clients.size(); ++i)
						{
							packet.clear();
							packet << std::uint32_t(this->localHostPlayers.size());
							if (this->clients[i]->send(packet) == sf::Socket::Done)
							{
								packet.clear();
								for (std::size_t j = 0; j < this->localHostPlayers.size(); ++j)
								{
									packet << this->localHostPlayers[j].first.toString() << this->localHostPlayers[j].second;
									if (this->clients[i]->send(packet) != sf::Socket::Done)
										std::cout << "Error! Cannot reach the client...\n";
									packet.clear();
								}
							}
							packet.clear();
						}
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
							sf::Lock lock(this->mutex);
							if (client.receive(packet) == sf::Socket::Done)
							{
								if (packet.getDataSize() == sizeof(std::uint8_t))
								{
									std::uint8_t data;
									packet >> data;
									packet.clear();

									switch (data)
									{
									case 52:
										for (auto it2 = this->localHostPlayers.begin(); it2 != this->localHostPlayers.end(); ++it2)
										{
											if (client.getRemoteAddress() == it2->first)
											{
												this->localHostPlayers.erase(it2);
												this->localHostPlayers.shrink_to_fit();
												break;
											}
										}
										break;
									default:
										std::cout << "Client sent an unknown command...\n";
										break;
									}
								}
							}
							else
							{
								//std::cout << "Error! while receeving the packet...\n";
								std::cout << "Player disconnected: " << client.getRemoteAddress().toString() << ":" << client.getRemotePort() << '\n';
								this->selector.remove(client);
								client.disconnect();
								this->clients.erase(it);
								this->clients.shrink_to_fit();
								for (auto it2 = this->localHostPlayers.begin(); it2 != this->localHostPlayers.end(); ++it2)
								{
									if (client.getRemoteAddress() == it2->first)
									{
										this->localHostPlayers.erase(it2);
										this->localHostPlayers.shrink_to_fit();
										break;
									}
								}
								for (std::size_t i = 0; i < this->clients.size(); ++i)
								{
									packet.clear();
									packet << std::uint32_t(this->localHostPlayers.size());
									if (this->clients[i]->send(packet) == sf::Socket::Done)
									{
										packet.clear();
										for (std::size_t j = 0; j < this->localHostPlayers.size(); ++j)
										{
											packet << this->localHostPlayers[j].first.toString() << this->localHostPlayers[j].second;
											if (this->clients[i]->send(packet) != sf::Socket::Done)
												std::cout << "Error! Cannot reach the client...\n";
											packet.clear();
										}
									}
									packet.clear();
								}
								break;
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

const bool menu::shutdownServer()
{
	if (this->serverThread != nullptr)
	{
		this->serverThread->terminate();
		this->serverThread.release();
		this->selector.clear();
		this->hosting.close();

		sf::Packet packet;
		packet << std::uint8_t(52);
		for (std::size_t i = 0; i < this->clients.size(); ++i)
		{
			if (this->clients[i]->send(packet) != sf::Socket::Done)
				std::cout << "Error! Cannot reach the client...\n";
		}
		if (this->socket.send(packet) != sf::Socket::Done)
			std::cout << "Error! Cannot send quit msg to the server...\n";
		packet.clear();
		return true;
	}
	return false;
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

void menu::updateLocalServerNum()
{
	if (this->serverThread == nullptr)
	{
		while (this->m_window)
		{
			sf::Packet packet;
			if (this->socket.receive(packet) == sf::Socket::Done)
			{
				if (packet.getDataSize() == sizeof(std::uint8_t))
				{
					std::uint8_t data;
					packet >> data;
					packet.clear();

					switch (data)
					{
					case 52:
						std::cout << "Host disconnected...\n";
						this->socket.disconnect();
						this->socket.connect(this->serverIP, this->serverPort);
						this->localHostPlayers.clear();
						this->LocalHostPlayerNum = 0;
						this->refreshServerList();
						this->m_State = state::Multiplayer;
						this->m_ServerError = true;
						return;
					default:
						std::cout << "Client sent an unknown command...\n";
						break;
					}
				}
				else
				{
					sf::Lock lock(this->mutex);
					this->LocalHostPlayerNum = 0;
					this->localHostPlayers.clear();
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
						}
						packet.clear();
					}
				}
			}
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