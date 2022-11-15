#include "menu.h"

menu::menu(window& w) noexcept : m_window(w) 
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

const void menu::init(sf::RenderWindow& window)
{
	ImGui::SFML::Init(window);
	gui::loadStyle();
	gui::loadFont();

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

	this->frontImage.setTexture(resourceSystem::c_get<sf::Texture>("FrontImage"));
	this->frontImage.setTextureRect(sf::IntRect(sf::Vector2i(600 * settings::m_currFrontPicture, 0), sf::Vector2i(600, 600)));

	this->icon.setTexture(resourceSystem::c_get<sf::Texture>("Icon"));
	this->icon.setTextureRect(sf::IntRect(sf::Vector2i(100 * settings::m_currProfilePicture, 0), sf::Vector2i(100, 100)));

	this->backgroundImage.setTexture(&resourceSystem::c_get<sf::Texture>("Background"));
	this->backgroundImage.setSize(sf::Vector2f(window.getSize()));
	this->backgroundImage.setTextureRect(sf::IntRect(sf::Vector2i(1920 * settings::m_currBackgroundPicture, 0),
													 sf::Vector2i(1920, 1080)));
	this->backgroundImage.setFillColor(sf::Color(255, 255, 255, 200));

	this->m_MainMusic = resourceSystem::get<std::unique_ptr<sf::Music>>(settings::m_currMusic).get();
	this->m_MainMusic->setLoop(true);
	if (settings::m_MusicVolume > 0.f)
		this->m_MainMusic->play();
	this->m_MainMusic->setVolume(settings::m_MusicVolume);

	if (settings::rememberToStayLogedIn)
		this->m_State = state::MainMenu;
}

const void menu::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	ImGui::SFML::Update(window, dt);

	if (settings::m_MusicVolume == 0.f)
		this->m_MainMusic->pause();
	else if (settings::m_MusicVolume > 0.f && (this->m_MainMusic->getStatus() == sf::SoundSource::Paused || this->m_MainMusic->getStatus() == sf::SoundSource::Stopped))
		this->m_MainMusic->play();

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

		if (settings::m_ShowFPS)
		{
			ImGui::SetCursorPos(ImVec2(vMax.x * 0.9f, vMin.y));
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
		{
			ImGui::SetCursorPos(ImVec2(vMax.x - 750.f, vMin.y + 40.f));
			if (this->m_MainMusic->getStatus() == sf::SoundSource::Playing)
			{
				if (ImGui::ImageButton(resourceSystem::c_get<sf::Texture>("Pause"), sf::Vector2f(35.f, 35.f)))
					this->m_MainMusic->pause();
			}
			else
				if (ImGui::ImageButton(resourceSystem::c_get<sf::Texture>("Resume"), sf::Vector2f(35.f, 35.f)))
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
							this->m_MainMusic = resourceSystem::get<std::unique_ptr<sf::Music>>(settings::m_currMusic).get();
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
					stateSystem::add<game>(this->m_window);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Starts the game state.");

				ImGui::SetCursorPos(ImVec2(WidgetPos.x + 305.f, WidgetPos.y + 45.f));
				if (ImGui::Button("Multiplayer", ImVec2(200.f, 50.f)))
				{
					/*
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
					*/
				}
			}
			break;
		}
		case state::Settings:
		{
			if (ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
			{
				ImGui::SetWindowSize("Settings", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Settings", ImVec2(50.f, 50.f));

				if (this->m_ChangeKeybindigs)
				{
					if (ImGui::Begin("Change Keybindigs", &this->m_ChangeKeybindigs, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
					{
						ImGui::SetWindowFocus("Change Keybindigs");
						ImGui::SetWindowSize("Change Keybindigs", ImVec2(300.f, 200.f));

						vMin = ImGui::GetWindowContentRegionMin();
						vMax = ImGui::GetWindowContentRegionMax();

						std::string text = "Change [" + inputSystem::keyToString(this->m_ToChange.second) + "] to [" + inputSystem::keyToString(this->key) + "]";
						ImGui::TextColored(ImVec4(1, 0, 0, 1), text.c_str());
						for (std::size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
						{
							if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
							{
								this->key.m_InputType = InputType::KeyboardInput;
								this->key.m_KeyCode = static_cast<sf::Keyboard::Key>(i);
								if (inputSystem::eventToString(this->m_ToChange.second) == "Press")
									this->key.m_EventType = sf::Event::KeyPressed;
								else if (inputSystem::eventToString(this->m_ToChange.second) == "Release")
									this->key.m_EventType = sf::Event::KeyReleased;
								else
									ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error));
							}
						}
						for (std::size_t i = 0; i < sf::Mouse::ButtonCount; ++i)
						{
							if (sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i)) && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
							{
								this->key.m_InputType = InputType::MouseInput;
								this->key.m_MouseButton = static_cast<sf::Mouse::Button>(i);
								if (inputSystem::eventToString(this->m_ToChange.second) == "Press")
									this->key.m_EventType = sf::Event::MouseButtonPressed;
								else if (inputSystem::eventToString(this->m_ToChange.second) == "Release")
									this->key.m_EventType = sf::Event::MouseButtonReleased;
								else if (inputSystem::eventToString(this->m_ToChange.second) == "Move")
									this->key.m_EventType = sf::Event::MouseMoved;
								else
									ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error));
							}
						}
						for (std::uint32_t i = 0; i < sf::Joystick::Count; ++i)
						{
							if (sf::Joystick::isConnected(i))
							{
								for (std::uint32_t j = 0; j < sf::Joystick::ButtonCount; ++j)
								{
									if (sf::Joystick::isButtonPressed(i, j))
									{
										if (this->m_ToChange.second.m_InputType == InputType::JoystickButtonInput)
										{
											this->key.m_InputType = InputType::JoystickButtonInput;
											this->key.m_joystickButton = j;
											if (inputSystem::eventToString(this->m_ToChange.second) == "Press")
												this->key.m_EventType = sf::Event::JoystickButtonPressed;
											else if (inputSystem::eventToString(this->m_ToChange.second) == "Release")
												this->key.m_EventType = sf::Event::JoystickButtonReleased;
											else if (inputSystem::eventToString(this->m_ToChange.second) == "Move")
												this->key.m_EventType = sf::Event::JoystickMoved;
											else
												ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error));
										}
									}
								}
								for (std::size_t j = 0; j < sf::Joystick::AxisCount; ++j)
								{
									sf::Joystick::Axis tempAxis = static_cast<sf::Joystick::Axis>(j);
									if (sf::Joystick::getAxisPosition(i, tempAxis) > 0.f)
									{
										this->key.m_InputType = InputType::JoystickAxisInput;
										this->key.m_JoystickAxis = tempAxis;
										if (inputSystem::eventToString(this->m_ToChange.second) == "Press")
											this->key.m_EventType = sf::Event::JoystickButtonPressed;
										else if (inputSystem::eventToString(this->m_ToChange.second) == "Release")
											this->key.m_EventType = sf::Event::JoystickButtonReleased;
										else if (inputSystem::eventToString(this->m_ToChange.second) == "Move")
											this->key.m_EventType = sf::Event::JoystickMoved;
										else
											ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error));
									}
								}
							}
						}

						ImGui::SetCursorPos(ImVec2(vMin.x, vMax.y - 30.f));
						if (ImGui::Button("Cancel##Change Keybindigs", ImVec2(100.f, 30.f)) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
							this->m_ChangeKeybindigs = false;
						ImGui::SetCursorPos(ImVec2(vMax.x - 100.f, vMax.y - 30.f));
						if (ImGui::Button("OK##Change Keybindigs", ImVec2(100.f, 30.f)) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
						{
							this->m_ChangeKeybindigs = false;
							inputSystem::saveInput({ this->m_ToChange.first, this->key });
						}
						ImGui::End();
					}
				}

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
				{
					if (ImGui::InputText("Account name: ", this->myAccount.account_name, MAX_CHAR_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
						std::cout << "New account name: " << this->myAccount.account_name << '\n';
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Set your account name here.\nYou can just type it and it will be automaticaly saved.\nMaximum characters that are allowed is 16.");
					break;
				}
				case settingState::Input:
				{
					for (auto& it : inputSystem::getInputHandler())
					{
						ImGui::Text(it.first.c_str());
						ImGui::SameLine();
						ImGui::PushID(std::string(it.first + inputSystem::keyToString(it.second)).c_str());
						if (ImGui::Button(inputSystem::keyToString(it.second).c_str(), ImVec2(300.f, 30.f)))
						{
							this->m_ToChange = it;
							this->m_ChangeKeybindigs = true;
						}
						ImGui::PopID();
						ImGui::SameLine();
						ImGui::SetNextItemWidth(200.f);
						ImGui::PushID(it.first.c_str() + static_cast<std::uint32_t>(it.second.m_InputType));
						if (ImGui::BeginCombo("###PressOrRelease", inputSystem::eventToString(it.second).c_str(), ImGuiComboFlags_HeightSmall))
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
				}
				case settingState::Graphics:
				{
					if (ImGui::Checkbox("Fullscreen: ", &settings::m_Fullscreen))
						this->m_window.create(settings::m_Videomodes[settings::m_currVideomode], "Fan Game!", settings::m_Fullscreen, settings::m_FpsLimit, settings::m_Vsync);

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
								this->m_window.create(settings::m_Videomodes[settings::m_currVideomode], "Fan Game!", settings::m_Fullscreen, settings::m_FpsLimit, settings::m_Vsync);
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
							this->m_window.setVSync(settings::m_Vsync);

					if (ImGui::Checkbox("FPS Limit: ", &settings::m_isFPSLimited))
					{
						if (!settings::m_isFPSLimited)
							settings::m_FpsLimit = 0;
						this->m_window.setFramerateLimit(settings::m_FpsLimit);
					}

					if (!settings::m_Vsync)
					{
						if (ImGui::SliderInt("##Limit: ", &settings::m_FpsLimit, 30, 240))
							this->m_window.setFramerateLimit(settings::m_FpsLimit);
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
						this->frontImage.setTextureRect(sf::IntRect(sf::Vector2i(600 * settings::m_currFrontPicture, 0),
																	sf::Vector2i(600, 600)));
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
				ImGui::SetCursorPos(ImVec2(vMax.x - 350.f, vMax.y - 125.f));
				if (ImGui::Button("Back & Save##Settings", ImVec2(300.f, 75.f)))
				{
					if (settings::saveSettings("res/Settings.ini"))
						this->m_State = state::MainMenu;
				}
			}ImGui::End();
			break;
		}
		case state::Characters:
		{
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
					ImGui::Columns(static_cast<int>(this->characters.size() / 2) + 1, "Characters");
					for (std::size_t i = 0; i < this->characters.size(); ++i)
					{
						if (i != 0 && !(i % 2))
							ImGui::NextColumn();

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
							ImGui::TextColored(ImVec4(1.f, 0.0f, 0.1f, 1.f), "Can be an image of any ability or something like that.");
							ImGui::TextColored(ImVec4(0.f, 0.8f, 0.1f, 1.f), "This will be updated later so it displays information\nabout the charater that is currently being hoverd.");
							ImGui::EndTooltip();
						}
						ImGui::SameLine();
						ImGui::Text("Some information\nabout the\ncharacter that is\nbeing displayed\ncan go here.");

						if (!this->characters[i].unlocked)
						{
							ImGui::PushID("CharacterUnlock" + i);
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
		}
		case state::Multiplayer:
		{
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
				{
					/*
					this->shutdownServer();
					this->servers.clear();
					this->socket.disconnect();
					*/
					this->m_State = state::MainMenu;
				}
			}ImGui::End();
			break;
		}
		case state::Singleplayer:
		{
			if (ImGui::Begin("Singleplayer", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Singleplayer", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Singleplayer", ImVec2(50.f, 50.f));
				ImGui::End();
			}
			break;
		}
		case state::MultiLobby:
		{
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
			}ImGui::End();
			break;
		}
		case state::Login:
		{
			this->loginPanel(window, dt);
			break;
		}
		default:
		{
			ImGui::Text("You're not suposed to see this LOL...\nSomething went horibly wrong...");
			ImGui::LogButtons();
			if (ImGui::Button("Help me!"))
				this->m_State = state::MainMenu;
			break;
		}
		}
		ImGui::End();
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
	if (this->m_State == state::MainMenu && settings::logged_in)
	{
		window.draw(this->xp_bar);
		window.draw(this->curr_xp);
	}
	window.setView(window.getDefaultView());
	ImGui::SFML::Render(window);
	window.draw(this->pS);
}

const void menu::loginPanel(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	panel loginPanel("Login", window);

	static bool particleSystem = false;
	if (ImGui::Button("Particle system test"))
	{
		particleSystem = true;
		ImGui::OpenPopup("Particle System");
	}
	ImGui::Button("World create test");
	ImGui::Button("Animation test");

	if (ImGui::InputTextWithHint("User Name:", "Enter your user name here...", &settings::ProfileNickname, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank) ||
		ImGui::InputTextWithHint("Password:", "Enter your password here...", &settings::ProfilePassword, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank) ||
		ImGui::Button("Login"))
		this->login(settings::ProfileNickname, settings::ProfilePassword);
	ImGui::SameLine();
	if (ImGui::Button("Create Account"))
	{
		this->createAccountPanel = true;
		ImGui::OpenPopup("Create Account");
	}
	ImGui::SameLine();
	ImGui::Checkbox("Remember Me!", &settings::rememberToStayLogedIn);

	if (ImGui::BeginPopupModal("Particle System", &particleSystem))
	{
		static float angle_x = 1.f;
		static float angle_y = 1.f;
		static float randomNess = 3.f;
		ImGui::DragFloat("AngleX: ", &angle_x, 0.1f, -1.f, 1.f);
		ImGui::DragFloat("AngleY: ", &angle_y, 0.1f, -1.f, 1.f);
		ImGui::DragFloat("Randomness: ", &randomNess, 0.1f, 1.f, 10.f);
		ImGui::Text("Number of particles: %u", this->pS.getSize());
		ImVec2 pos = ImGui::GetCursorScreenPos();
		std::random_device rd;
		std::uniform_real_distribution<float> dist(0.f, randomNess);
		this->pS.createParticle(1, 1.f + dist(rd), 
								sf::Color(std::uint8_t(85 * dist(rd)), std::uint8_t(85 * dist(rd)), std::uint8_t(85 * dist(rd))), 
								sf::Vector2f(pos.x, pos.y), sf::Vector2f(angle_x * dist(rd), angle_y * dist(rd)));
		this->pS.update();
		ImGui::EndPopup();
	}

	if (this->m_gui.beginPopup("Create Account", this->createAccountPanel, window))
	{
		ImGui::Text("Create Account: ");
		if (ImGui::InputTextWithHint("Email:", "Enter your email here...", &this->createAccountEmail, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank) ||
			ImGui::InputTextWithHint("Username:", "Enter your name here...", &this->createAccountName, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank) ||
			ImGui::InputTextWithHint("Password:", "Enter your password here...", &this->createAccountPassword, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank) ||
			ImGui::Button("Create"))
			this->createAccount(this->createAccountName, this->createAccountPassword, this->createAccountEmail);
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();
		this->m_gui.endPopup();
	}
}

const bool menu::login(const std::string& name, const std::string& password) noexcept
{
	this->m_client.join();
	this->m_client.send(static_cast<std::uint32_t>(Network_MSG::LogInAttempt), name, password).wait();
	bool resoult = false;
	using namespace std::chrono_literals;
	this->m_client.receive(resoult).wait_for(1s);
	if (resoult)
	{
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Succesfuly logged in as %s", this->myAccount.account_name));
		settings::logged_in = true;
		this->m_State = state::MainMenu;
	}
	else
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Incorrect username or password!"));
	this->m_client.disconnect();
	return resoult;
}

const bool menu::createAccount(const std::string& name, const std::string& password, const std::string& email) noexcept
{
	bool resoult = false;
	if (email.find('@') != std::string::npos && email.find('.') != std::string::npos)
	{
		this->m_client.join();
		this->m_client.send(static_cast<std::uint32_t>(Network_MSG::RegisterAttempt), name, password, email).wait();
		using namespace std::chrono_literals;
		this->m_client.receive(resoult).wait_for(1s);
		if (resoult)
		{
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Account succesfuly created!"));
			settings::logged_in = true;
			this->m_State = state::MainMenu;
		}
		else
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Cannot create account with given detailes!"));
		this->m_client.disconnect();
	}
	else
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Cannot create account, provided email is not valid!"));
	return resoult;
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