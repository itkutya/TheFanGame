#include "menu.h"

menu::menu(window& window) noexcept 
{ 
	this->m_window = &window;
}

menu::~menu() noexcept { ImGui::SFML::Shutdown(); }

const void menu::init(sf::RenderWindow& window)
{
	ImGui::SFML::Init(window);
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.f;
	style.FrameRounding = 15.3f;
	style.ScrollbarRounding = 2.5f;
	style.GrabRounding = 2.5f;

	//Bruh maybie not here? idk...
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

	this->m_videomodes = sf::VideoMode::getFullscreenModes();
	this->backgroundImage.setTexture(&this->m_window->getTexture(0));
	this->backgroundImage.setSize(sf::Vector2f(window.getSize()));
	this->backgroundImage.setTextureRect(sf::IntRect(64 * this->currBackgroundPicture, 0, 64, 64));
	this->backgroundImage.setFillColor(sf::Color(255, 255, 255, 125));

	this->xp_bar.setPosition(sf::Vector2f(190.f, 50.f));
	this->xp_bar.setSize(sf::Vector2f(300.f, 10.f));
	this->xp_bar.setFillColor(sf::Color::White);
	this->xp_bar.setOutlineThickness(2.f);
	this->xp_bar.setOutlineColor(sf::Color::Black);

	this->curr_xp.setPosition(this->xp_bar.getPosition());
	this->curr_xp.setSize(sf::Vector2f(this->xp_bar.getSize().x * this->xp, this->xp_bar.getSize().y));
	this->curr_xp.setFillColor(sf::Color::Yellow);
	this->curr_xp.setOutlineThickness(0.f);
	this->curr_xp.setOutlineColor(sf::Color::Yellow);

	this->m_view.setSize(sf::Vector2f(window.getSize()));
	this->m_view.setCenter(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));

	for (std::size_t i = 0; i < 5; ++i)
	{
		this->characters.emplace_back();
	}
}

const void menu::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	ImGui::SFML::Update(window, dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		this->giveXP(10.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		this->currency += 10;

	if (ImGui::Begin("Main Window", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground))
	{
		ImGui::SetWindowSize("Main Window", ImVec2((float)window.getSize().x, (float)window.getSize().y));
		ImGui::SetWindowPos("Main Window", ImVec2(0.f, 0.f));

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 100.f, 25.f));
		ImGui::Text("FPS: %.3f", 1.f / dt.asSeconds());
		
		if (!this->settingsPanel && !this->charactersPanel)
		{
			this->createImage(this->m_window->getTexture(0), sf::IntRect(64 * this->currProfilePicture, 0, 64, 64), sf::Vector2f(25.f, 30.f), sf::Vector2f(50.f, 50.f));
			this->setToolTip("This is the profile picture!");
			ImGui::SetCursorPos(ImVec2(80.f, 22.5f));
			ImGui::BeginGroup();
			ImGui::Text("Account: %s", this->account_name.c_str());
			ImGui::Text("XP: %.0f / %.0f", this->xp, this->xp_cap);
			ImGui::Text("Level: %i", this->account_lvl);
			ImGui::Text("CoverCoin: %ic", this->currency);
			ImGui::EndGroup();

			this->createImage(this->m_window->getTexture(0), sf::IntRect(64 * this->currFrontPicture, 0, 64, 64), sf::Vector2f(300.f, 175.f), sf::Vector2f(300.f, 300.f));
			this->setToolTip("This is the front image!");

			if (this->createButton("Play", sf::Vector2f(50.f, window.getSize().y / 5.f), sf::Vector2f(200.f, 50.f)))
			{
				if (!this->play_selecter)
					this->play_selecter = true;
				else if (this->play_selecter)
					this->play_selecter = false;
			}

			if (this->play_selecter)
			{
				if (this->createButton("Singleplayer", sf::Vector2f(250.f, window.getSize().y / 5.f - 35.f), sf::Vector2f(200.f, 50.f)))
				{
					this->m_window->addState<game>();
					this->play_selecter = false;
				}
				this->setToolTip("Starts the game state.");
				if (this->createButton("Multiplayer", sf::Vector2f(250.f, window.getSize().y / 5.f + 35.f), sf::Vector2f(200.f, 50.f)))
				{
					std::cout << "Working on it...\n";
					this->multiplayerPanel = true;
					this->play_selecter = false;
				}
			}

			if (this->createButton("Characters", sf::Vector2f(50.f, window.getSize().y / 5.f + 75.f), sf::Vector2f(200.f, 50.f)))
				this->charactersPanel = true;
			this->setToolTip("Unlocked characters and the character shop, etc...");

			if (this->createButton("Settings", sf::Vector2f(50.f, window.getSize().y / 5.f + 150.f), sf::Vector2f(200.f, 50.f)))
				this->settingsPanel = true;
			this->setToolTip("Opens the setting menu.");

			if (this->createButton("Quit", sf::Vector2f(50.f, window.getSize().y / 5.f + 225.f), sf::Vector2f(200.f, 50.f)))
				window.close();
			this->setToolTip("Goodbye!");
		}

		if (this->settingsPanel && !this->charactersPanel)
		{
			if (ImGui::Begin("Settings Panel", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Settings Panel", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Settings Panel", ImVec2(25.f, 25.f));
				ImGui::SetWindowFocus("Settings Panel");

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
					if (ImGui::SliderInt("###Limit: ", &this->fps_limit, 30, 180))
						this->m_window->setFramerateLimit(this->fps_limit);
				}

				ImGui::SliderFloat("Sensivity: ", &this->sensivity, 0.f, 20.f);
				ImGui::SliderFloat("Game volume: ", &this->game_volume, 0.f, 100.f);
				ImGui::SliderFloat("Music volume: ", &this->music_volume, 0.f, 100.f);

				ImGui::SliderInt("Front image: ", &this->currFrontPicture, 0, 5);
				ImGui::SliderInt("Profile picture: ", &this->currProfilePicture, 0, 5);
				if (ImGui::SliderInt("Background image: ", &this->currBackgroundPicture, 0, 5))
					this->backgroundImage.setTextureRect(sf::IntRect(64 * this->currBackgroundPicture, 0, 64, 64));

				if (this->createButton("Back##Settings", sf::Vector2f(ImGui::GetWindowSize().x - 215.f, ImGui::GetWindowSize().y - 75.f), sf::Vector2f(200.f, 50.f)))
					this->settingsPanel = false;

				ImGui::End();
			}
		}

		if (this->charactersPanel && !this->settingsPanel)
		{
			if (ImGui::Begin("Characters Panel", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Characters Panel", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Characters Panel", ImVec2(25.f, 25.f));
				ImGui::SetWindowFocus("Characters Panel");

				int y = 0;
				int x = 0;
				for (std::size_t i = 0; i < this->characters.size(); ++i)
				{
					if (i != 0 && i % 8 == 0)
					{
						++x;
						y = 0;
					}
					if (!this->characters[i].unlocked)
						this->createImage(this->m_window->getTexture(0), sf::IntRect(64 * (int)i, 0, 64, 64), sf::Vector2f(25.f + 250.f * (float)x, 40.f + 75.f * (float)y), sf::Vector2f(50.f, 50.f));
					else
						this->createImage(this->m_window->getTexture(1), sf::IntRect(64 * (int)i, 0, 64, 64), sf::Vector2f(25.f + 250.f * (float)x, 40.f + 75.f * (float)y), sf::Vector2f(50.f, 50.f));

					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						this->createImage(this->m_window->getTexture(0), sf::IntRect(128, 0, 64, 64), sf::Vector2f(5.f, 5.f), sf::Vector2f(50.f, 50.f));
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1.f, 0.0f, 0.1f, 1.f), "Can be an image of any ability or something like that.");
						ImGui::TextColored(ImVec4(0.f, 0.8f, 0.1f, 1.f), "This will be updated later so it displays information\nabout the charater that is currently being hoverd.");
						ImGui::EndTooltip();
					}
					ImGui::SameLine();
					ImGui::Text("Some information\nabout the\ncharacter that is\nbeing displayed\ncan go here.");

					if (!this->characters[i].unlocked)
					{
						ImGui::PushID((int)i);
						if (this->createButton("Unlock", sf::Vector2f(25.f + 250.f * (float)x, 92.5f + 75.f * (float)y), sf::Vector2f(50.f, 20.f)))
						{
							if (this->currency > 699)
							{
								this->currency -= 699;
								this->characters[i].unlocked = true;
							}
						}
						if (ImGui::IsItemHovered())
						{
							if (this->currency < 699)
							{
								ImGui::BeginTooltip();
								ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Not enough currency, come back later.\nAnd put some usefull information here later...");
								ImGui::EndTooltip();
							}
							else
							{
								ImGui::BeginTooltip();
								ImGui::TextColored(ImVec4(0.f, 1.f, 0.4f, 1.f), "This item costs 699c.");
								ImGui::EndTooltip();
							}
						}
						ImGui::PopID();
					}
					++y;
				}

				if (this->createButton("Unlock more characters", sf::Vector2f(25.f, ImGui::GetWindowSize().y - 75.f), sf::Vector2f(200.f, 50.f)))
				{
					if (this->currency >= 420)
					{
						this->currency -= 420;
						std::cout << "Will do it later... But this will cost less than stuff above...\n";
					}
				}
				if (ImGui::IsItemHovered())
				{
					if (this->currency < 420)
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

				if (this->createButton("Back##Characters", sf::Vector2f(ImGui::GetWindowSize().x - 215.f, ImGui::GetWindowSize().y - 75.f), sf::Vector2f(200.f, 50.f)))
					this->charactersPanel = false;
				
				ImGui::End();
			}
		}

		if (this->multiplayerPanel)
		{
			if (ImGui::Begin("Multiplayer Panel", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Multiplayer Panel", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Multiplayer Panel", ImVec2(25.f, 25.f));
				ImGui::SetWindowFocus("Multiplayer Panel");

				if (ImGui::BeginListBox("Servers", ImVec2(ImGui::GetWindowSize().x - 100.f, ImGui::GetWindowSize().y - 200.f)))
				{
					for (size_t i = 0; i < 20; i++)
					{
						ImGui::PushID(i);
						if (ImGui::Selectable("Server with an IP... blah blah blah 127.0.0.0.0"))
							std::cout << "connecting to the server... " << i << "\n";
						ImGui::PopID();
					}
					ImGui::EndListBox();
				}

				if (this->createButton("Host", sf::Vector2f(25.f, ImGui::GetWindowSize().y - 125.f), sf::Vector2f(100.f, 25.f)))
					std::cout << "Hosting...\n";

				ImGui::SetCursorPos(ImVec2(25.f, ImGui::GetWindowSize().y - 150.f));
				if (ImGui::InputTextWithHint("Server IP: ", "127.0.0.0", this->buffer, 12))
					std::cout << this->buffer << "\n";
				
				if (this->createButton("Join", sf::Vector2f(175.f, ImGui::GetWindowSize().y - 125.f), sf::Vector2f(100.f, 25.f)))
					std::cout << "Joining...\n";

				if (this->createButton("Back##Multiplayer", sf::Vector2f(ImGui::GetWindowSize().x - 215.f, ImGui::GetWindowSize().y - 75.f), sf::Vector2f(200.f, 50.f)))
					this->multiplayerPanel = false;

				ImGui::End();
			}

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

const void menu::giveXP(const float& amount) noexcept
{
	this->xp += amount;
	while (this->xp >= this->xp_cap)
	{
		this->xp -= this->xp_cap;
		this->xp_cap += (10.f * this->account_lvl);
		++this->account_lvl;
	}
	this->curr_xp.setSize(sf::Vector2f(this->xp_bar.getSize().x * (this->xp / this->xp_cap), this->xp_bar.getSize().y));
}

const bool menu::createButton(const char* name, const sf::Vector2f& pos, const sf::Vector2f& size) noexcept
{
	ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
	if (ImGui::Button(name, ImVec2(size.x, size.y)))
		return true;
	return false;
}

const void menu::createImage(const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& pos, const sf::Vector2f& size) noexcept
{
	sf::Sprite temp;
	temp.setTexture(texture);
	temp.setTextureRect(rect);
	ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
	ImGui::Image(temp, size);
}

const bool menu::setToolTip(const char* text) noexcept
{
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(text);
		return true;
	}
	return false;
}
