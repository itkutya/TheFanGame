#include "MainScreen.h"

void MainScreen::init(sf::RenderWindow& window)
{
	this->m_Icon = this->s_ResourceManager->add<ResourceManager::Entity>("Icon");
	if (this->m_Icon->Texture.loadFromFile("Resources/Icons.png"))
	{
		this->m_Icon->Sprite.setTexture(this->m_Icon->Texture);
		this->m_Icon->Sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 100, 100 }));
	}

	this->m_FrontImage = this->s_ResourceManager->add<ResourceManager::Entity>("FrontImage");
	if (this->m_FrontImage->Texture.loadFromFile("Resources/FontImages.png"))
	{
		this->m_FrontImage->Sprite.setTexture(this->m_FrontImage->Texture);
		this->m_FrontImage->Sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 600, 600 }));
	}

	this->m_Music = this->s_ResourceManager->add<sf::Music>("MainScreenMusic");
	if (this->m_Music->openFromFile("Resources/" + this->m_CurrentMusicTitle + ".wav"))
	{
		this->m_Music->setLoop(true);
		this->m_Music->setVolume(20.f);
		this->m_Music->play();
	}

	this->m_Pause = this->s_ResourceManager->add<sf::Texture>("PauseTexture");
	this->m_Resume = this->s_ResourceManager->add<sf::Texture>("ResumeTexture");
	if (!this->m_Pause->loadFromFile("Resources/Pause.png") || !this->m_Resume->loadFromFile("Resources/Resume.png"))
		std::printf("Erro could not load some textures!");
}

void MainScreen::processEvent(const sf::Event& event) noexcept
{
}

void MainScreen::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	if (ImGui::Begin("##MainScreen", 0, flags))
	{
		auto& io = ImGui::GetIO();
		ImGui::SetCursorPos(ImVec2((ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x) * 0.85f,
									ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y));
		ImGui::TextColored(ImVec4(0, 0, 1, 1), "%.2f fps (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

		if (ImGui::BeginTable("MainMenu", 2))
		{
			ImGui::TableNextColumn();
			if (ImGui::BeginTable("MainMenuProfile", 2, ImGuiTableFlags_SizingFixedFit))
			{
				ImGui::TableNextColumn();
				ImGui::Image(this->m_Icon->Sprite, sf::Vector2f(100.f, 100.f));
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This is the profile picture!");
				ImGui::TableNextColumn();
				ImGui::Text("Account: %s\nXP: %.2f%%\nLevel: %i\nCoverCoin: %i$",
					this->s_Account->m_username.c_str(),
					this->s_Account->m_Experience.getProgress() * 100.f,
					this->s_Account->m_Experience.getLevel(),
					0);
				ImGui::Dummy(ImVec2(ImGui::GetContentRegionMax().x, 0.f));
				ImGui::ProgressBar(this->s_Account->m_Experience.getProgress());
				ImGui::EndTable();
			}
			ImGui::TableNextColumn();
			if (this->m_Music->getStatus() == sf::SoundSource::Playing)
			{
				if (ImGui::ImageButton(*this->m_Pause, sf::Vector2f(35.f, 35.f)))
					this->m_Music->pause();
			}
			else
				if (ImGui::ImageButton(*this->m_Resume, sf::Vector2f(35.f, 35.f)))
					this->m_Music->play();
			ImGui::SameLine();
			ImGui::Text("Currently playing: ");
			ImGui::SameLine();
			if (ImGui::BeginCombo("###MusicSelector", this->m_CurrentMusicTitle.c_str(), ImGuiComboFlags_HeightSmall))
			{
				for (auto& music : this->m_MusicTitles)
				{
					if (ImGui::Selectable(music.c_str()))
					{
						if (this->m_CurrentMusicTitle != music)
						{
							this->m_CurrentMusicTitle = music;
							this->m_Music->stop();
							if (this->m_Music->openFromFile("Resources/" + music + ".wav"))
							{
								this->m_Music->setVolume(20.f);
								this->m_Music->play();
							}
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
						this->m_PlaySelected = false;
						//this->m_MainMusic->stop();
						//this->m_State = state::Singleplayer;
						//stateSystem::add<game>(window);
					}
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Starts the game state.");

					if (ImGui::Button("Multiplayer", ImVec2(200.f, 50.f)))
					{
						/*
						if (this->mpPanel.network.join())
						{
							using namespace std::chrono_literals;
							if (this->mpPanel.network.receive(this->mpPanel.ServerNum).wait_for(3s) == std::future_status::ready)
							{
								for (std::size_t i = 0; i < this->mpPanel.ServerNum; ++i)
									this->mpPanel.servers.push_back({ sf::IpAddress(0), 0 });
								guistateSystem::add<multiplayerPanel>();
							}
							else
								ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, "Cannot connect to main server!"));
						}
						else
							ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, "Cannot connect to main server!"));
						*/
					}
					ImGui::EndPopup();
				}

				ImGui::TableNextRow(0, 100.f);
				ImGui::TableNextColumn();
				ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 3.f);

				if (ImGui::Button("Characters", ImVec2(300.f, 75.f)))
					//guistateSystem::add<charactersPanel>();
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Unlocked characters and the character shop, etc...");

				ImGui::TableNextRow(0, 100.f);
				ImGui::TableNextColumn();
				ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 3.f);

				if (ImGui::Button("Settings", ImVec2(300.f, 75.f)))
					//guistateSystem::add<settingsPanel>();
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Opens the setting menu.");

				ImGui::TableNextRow(0, 100.f);
				ImGui::TableNextColumn();
				ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 3.f);

				if (ImGui::Button("Quit", ImVec2(300.f, 75.f)))
					window.close();
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Goodbye!");
				ImGui::EndTable();
			}
			ImGui::TableNextColumn();
			ImGui::Image(this->m_FrontImage->Sprite, sf::Vector2f(ImGui::GetContentRegionAvail().x - 5.f, ImGui::GetContentRegionAvail().y - 5.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("This is the front image!");
			ImGui::EndTable();
		}
	}
	ImGui::End();
}

void MainScreen::draw(sf::RenderWindow& window) noexcept
{
}
