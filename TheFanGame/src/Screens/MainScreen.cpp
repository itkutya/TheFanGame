#include "MainScreen.h"

void MainScreen::init(sf::RenderWindow& window)
{
	this->m_Icon = this->s_ResourceManager.add<ResourceManager::Object>("Icon");
	if (this->m_Icon->Texture.loadFromFile("Resources/Icons.png"))
	{
		this->m_Icon->Sprite.setTexture(this->m_Icon->Texture);
		this->m_Icon->Sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 100, 100 }));
	}

	this->m_FrontImage = this->s_ResourceManager.add<ResourceManager::Object>("FrontImage");
	if (this->m_FrontImage->Texture.loadFromFile("Resources/FontImages.png"))
	{
		this->m_FrontImage->Sprite.setTexture(this->m_FrontImage->Texture);
		this->m_FrontImage->Sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 600, 600 }));
	}

	if (!this->s_AudioManager.replaceCurrentMusic(this->s_AudioManager.m_CurrentMusicTitle))
		ImGui::InsertNotification({ ImGuiToastType_Warning, "Failed to load the music file!" });
}

void MainScreen::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	if (ImGui::Begin("##MainScreen", 0, flags))
	{
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
				ImGui::Text("Account: %s\nXP: %.2f/%.2f\nLevel: %i\nCoverCoin: %u$",
					(this->s_Account.m_username).c_str(),
					 this->s_Account.m_experience.getCurrentXP(),
					 this->s_Account.m_experience.getCurrentXPCap(),
					 this->s_Account.m_experience.getLevel(),
					 this->s_Account.m_currency);
				ImGui::Dummy(ImVec2(ImGui::GetContentRegionMax().x, 0.f));
				ImGui::ProgressBar(this->s_Account.m_experience.getProgress());
				ImGui::EndTable();
			}
			ImGui::TableNextColumn();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (this->s_AudioManager.m_CurrentMusic->getStatus() == sf::SoundSource::Playing)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
				if (ImGui::Button("\xef\x81\x8c", sf::Vector2f(35.f, 35.f)))
					this->s_AudioManager.m_CurrentMusic->pause();
			}
			else
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.65f, 0.5f));
				if (ImGui::Button("\xef\x81\x8b", sf::Vector2f(35.f, 35.f)))
					this->s_AudioManager.m_CurrentMusic->play();
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::SameLine();
			ImGui::Text("Currently playing: ");
			ImGui::SameLine();
			ImGui::PushItemWidth(300.f);
			if (ImGui::BeginCombo("###MusicSelector", (this->s_AudioManager.m_CurrentMusicTitle).c_str(), ImGuiComboFlags_HeightSmall))
			{
				for (auto& music : this->s_AudioManager.m_MusicTitles)
				{
					if (ImGui::Selectable(music.c_str()) && this->s_AudioManager.m_CurrentMusicTitle != music)
						if (!this->s_AudioManager.replaceCurrentMusic(music))
							ImGui::InsertNotification({ ImGuiToastType_Warning, "Failed to load the music file!" });
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

				if (ImGui::Button("Shop", ImVec2(300.f, 75.f)))
					this->s_StateManager.addGUIState<ShopScreen>(this->m_app);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Unlocked characters and the character shop, etc...");

				ImGui::TableNextRow(0, 100.f);
				ImGui::TableNextColumn();
				ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 3.f);

				if (ImGui::Button("Settings", ImVec2(300.f, 75.f)))
					this->s_StateManager.addGUIState<SettingsScreen>(this->m_app);
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