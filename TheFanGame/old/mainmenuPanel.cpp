#include "mainmenuPanel.h"

mainmenuPanel::mainmenuPanel() noexcept
{
	this->m_MainMusic = resourceSystem::get<std::unique_ptr<sf::Music>>(settings::m_currMusic).get();
}

const void mainmenuPanel::init(window& window) noexcept
{
}

const void mainmenuPanel::processEvent(const sf::Event& event) noexcept
{
}

const void mainmenuPanel::update(window& window, const sf::Time& dt) noexcept
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
			ImGui::Image(this->sPanel.m_icon, sf::Vector2f(100.f, 100.f));
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("This is the profile picture!");
			ImGui::TableNextColumn();
			ImGui::Text("Account: %s\nXP: %0.f / %0.f\nLevel: %i\nCoverCoin: %i$",
				profile::name,
				profile::xp, profile::xp_cap,
				profile::lvl,
				profile::currency);
			ImGui::Dummy(ImVec2(ImGui::GetContentRegionMax().x, 0.f));
			ImGui::ProgressBar(profile::xp / profile::xp_cap);
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
					this->m_PlaySelected = false;
					this->m_MainMusic->stop();
					//this->m_State = state::Singleplayer;
					//stateSystem::add<game>(window);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Starts the game state.");

				if (ImGui::Button("Multiplayer", ImVec2(200.f, 50.f)))
				{
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
				}
				ImGui::EndPopup();
			}

			ImGui::TableNextRow(0, 100.f);
			ImGui::TableNextColumn();
			ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 3.f);

			if (ImGui::Button("Characters", ImVec2(300.f, 75.f)))
				guistateSystem::add<charactersPanel>();
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Unlocked characters and the character shop, etc...");

			ImGui::TableNextRow(0, 100.f);
			ImGui::TableNextColumn();
			ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 3.f);

			if (ImGui::Button("Settings", ImVec2(300.f, 75.f)))
				guistateSystem::add<settingsPanel>();
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Opens the setting menu.");

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
		ImGui::Image(this->sPanel.m_frontImage, sf::Vector2f(ImGui::GetContentRegionAvail().x - 5.f, ImGui::GetContentRegionAvail().y - 5.f));
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("This is the front image!");
		ImGui::EndTable();
	}
}

const void mainmenuPanel::draw(window& window) noexcept
{
}