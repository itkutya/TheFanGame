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
	this->settingsPanel.frontImage = &this->frontImage;

	this->icon.setTexture(resourceSystem::c_get<sf::Texture>("Icon"));
	this->icon.setTextureRect(sf::IntRect(sf::Vector2i(100 * settings::m_currProfilePicture, 0), sf::Vector2i(100, 100)));
	this->settingsPanel.icon = &this->icon;

	this->backgroundImage.setTexture(resourceSystem::c_get<sf::Texture>("Background"));
	this->backgroundImage.setTextureRect(sf::IntRect({ 1920 * settings::m_currBackgroundPicture, 0 }, { 1920, 1080 }));
	this->backgroundImage.setScale(sf::Vector2f(window.getView().getSize().x / this->backgroundImage.getLocalBounds().width,
												window.getView().getSize().y / this->backgroundImage.getLocalBounds().height));
	this->backgroundImage.setColor(sf::Color(255, 255, 255, 200));
	this->settingsPanel.backgroundImage = &this->backgroundImage;

	this->m_MainMusic = resourceSystem::get<std::unique_ptr<sf::Music>>(settings::m_currMusic).get();
	this->m_MainMusic->setLoop(true);
	if (settings::m_MusicVolume > 0.f)
		this->m_MainMusic->play();
	this->m_MainMusic->setVolume(settings::m_MusicVolume);
	this->settingsPanel.m_MainMusic = this->m_MainMusic;

	if (settings::rememberToStayLogedIn)
		this->m_State = state::MainMenu;
}

const void menu::update(window& window, const sf::Time& dt) noexcept
{
	ImGui::SFML::Update(window.getWindow(), dt);

	switch (this->m_State)
	{
	case state::MainMenu:
		this->mainmenuPanel(window, dt);
		break;
	case state::Multiplayer:
		this->multiplayerPanel(window, dt);
		break;
	case state::Singleplayer:
		break;
	case state::MultiLobby:
		break;
	case state::Login:
		this->loginPanel(window, dt);
		break;
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
	{
		this->backgroundImage.setScale(sf::Vector2f((float)event.size.width / this->backgroundImage.getLocalBounds().width,
													(float)event.size.height / this->backgroundImage.getLocalBounds().height));
	}
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
	panel mainWindowPanel("Main Window", window.getWindow());

	if (settings::m_ShowFPS)
	{
		auto& io = ImGui::GetIO();
		mainWindowPanel.text("%.2f fps (%.2gms)", ImVec4(0, 0, 1, 1),
							 ImVec2((ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x) * 0.85f,
									 ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y),
									 io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
	}

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
				{
					if (this->network.join())
					{
						using namespace std::chrono_literals;
						if (this->network.receive(this->ServerNum).wait_for(3s) == std::future_status::ready)
						{
							for (std::size_t i = 0; i < this->ServerNum; ++i)
								this->servers.push_back({ sf::IpAddress(0), 0 });
							this->m_State = state::Multiplayer;
						}
						else
							ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, "Cannot connect to main server!"));
					}
					else
						ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, "Cannot connect to main server!"));
				}
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
				this->settingsPanel.settingsPopUpShouldBeOpen = true;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Opens the setting menu.");
			this->settingsPanel.upadte(window, dt);

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

const void menu::multiplayerPanel(window& window, const sf::Time& dt) noexcept
{
	panel MultiplayerPanel("Multiplayer", window.getWindow());

	//TODO: Make it not an int...
	static int menuItem = 0;
	if (ImGui::BeginChild(ImGuiID(2), ImVec2(ImGui::GetWindowSize().x - 100.f, ImGui::GetWindowSize().y - 275.f), true, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Local"))
			{
				menuItem = 1;
				this->refresLocal();
			}
			if (ImGui::MenuItem("Public"))
			{
				menuItem = 0;
				this->refresPublic();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginListBox("Servers", ImGui::GetContentRegionAvail()))
		{
			for (const auto& server : this->servers)
			{
				std::string serverInfo = server.first.toString() + ':' + std::to_string(server.second);
				if (ImGui::Selectable(serverInfo.c_str()))
					std::cout << "connecting to the server... " << serverInfo << "\n";
			}
			ImGui::EndListBox();
		}
		ImGui::EndChild();
	}

	if (MultiplayerPanel.button("Refress"))
	{
		if (menuItem)
			this->refresPublic();
		else
			this->refresLocal();
	}

	ImGui::SetNextItemWidth(200.f);
	if (ImGui::InputTextWithHint("Server IP", "127.0.0.0", this->InputIp, 12, ImGuiInputTextFlags_EnterReturnsTrue))
		std::cout << this->InputIp << "\n";
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);
	if (ImGui::InputTextWithHint("Port", "52420", this->InputPort, 6, ImGuiInputTextFlags_EnterReturnsTrue))
		std::cout << this->InputPort << "\n";
	ImGui::SameLine();
	if (MultiplayerPanel.button("Join", ImVec2(200.f, 50.f)))
		std::cout << "connecting to the server... " << this->InputIp << ":" << this->InputPort << "\n";

	if (MultiplayerPanel.button("Local Host", ImVec2(300.f, 75.f)))
	{
		this->host.start();
		this->m_State = state::MultiLobby;
	}
	ImGui::SameLine();
	if (MultiplayerPanel.button("Public Host", ImVec2(300.f, 75.f)))
	{
		this->host.start();
		this->m_State = state::MultiLobby;
	}

	if (ImGui::Button("Back##Multiplayer", ImVec2(300.f, 75.f)))
		this->m_State = state::MainMenu;
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
				if (x == 1)
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

const void menu::refresLocal() noexcept
{

}

const void menu::refresPublic() noexcept
{
	this->servers.clear();
	using namespace std::chrono_literals;
	if (this->network.receive(this->ServerNum).wait_for(3s) == std::future_status::ready)
	{
		for (std::size_t i = 0; i < this->ServerNum; ++i)
			this->servers.push_back({ sf::IpAddress(0), 0 });
	}
}