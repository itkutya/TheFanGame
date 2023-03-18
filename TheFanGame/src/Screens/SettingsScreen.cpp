#include "SettingsScreen.h"

void SettingsScreen::init(sf::RenderWindow& window)
{
	this->m_open = true;
}

void SettingsScreen::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	if (this->m_once)
	{
		ImGui::OpenPopup("Settings");
		this->m_once = false;
	}

	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Settings", &this->m_open, flags))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Main menu"))
				this->m_state = SETTINGS_STATE::MAINSCREEN;
			if (ImGui::Button("Profile"))
				this->m_state = SETTINGS_STATE::PROFILE;
			if (ImGui::Button("Game"))
				this->m_state = SETTINGS_STATE::GAME;
			if (ImGui::Button("Graphics"))
				this->m_state = SETTINGS_STATE::GRAPHICS;
			if (ImGui::Button("Input"))
				this->m_state = SETTINGS_STATE::INPUT;
			if (ImGui::Button("Audio"))
				this->m_state = SETTINGS_STATE::AUDIO;
			ImGui::EndMenuBar();
		}
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);

		switch (this->m_state)
		{
		case SETTINGS_STATE::GRAPHICS:
		{
			if (ImGui::Checkbox("Fullscreen: ", &this->m_app->m_fullscreen))
				this->m_app->recreateWindow();

			ImGui::SetNextItemWidth(300.f);
			std::string preview = std::to_string(this->m_app->m_videomodes[this->m_app->m_videomode].size.x) + "x" + std::to_string(this->m_app->m_videomodes[this->m_app->m_videomode].size.y);
			if (ImGui::BeginCombo("Resolution: ", preview.c_str(), ImGuiComboFlags_HeightSmall))
			{
				for (std::size_t i = 0; i < this->m_app->m_videomodes.size(); ++i)
				{
					std::string text = std::to_string(this->m_app->m_videomodes[i].size.x) + "x" + std::to_string(this->m_app->m_videomodes[i].size.y);
					if (ImGui::Selectable(text.c_str()))
					{
						this->m_app->m_videomode = (int)i;
						this->m_app->m_size = this->m_app->m_videomodes[i];
						this->m_app->recreateWindow();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::Checkbox("Show FPS", &this->m_app->m_showfps);

			if (!this->m_app->m_fps)
				if (ImGui::Checkbox("VSync: ", &this->m_app->m_vsync))
					window.setVerticalSyncEnabled(this->m_app->m_vsync);

			if (!this->m_app->m_vsync)
				if (ImGui::Checkbox("FPS Limit: ", &this->m_app->m_fps) || ImGui::SliderInt("##Limit: ", &this->m_app->m_fpslimit, 30, 240))
					window.setFramerateLimit(this->m_app->m_fps ? this->m_app->m_fpslimit : this->m_app->m_fps);
			break;
		}
		case SETTINGS_STATE::PROFILE:
			break;
		case SETTINGS_STATE::AUDIO:
		{
			ImGui::Text("Currently playing: ");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(500.f);
			if (ImGui::BeginCombo("###MusicSelector", this->s_AudioManager.m_CurrentMusicTitle.c_str(), ImGuiComboFlags_HeightSmall))
			{
				for (auto& music : this->s_AudioManager.m_MusicTitles)
				{
					if (ImGui::Selectable(music.c_str()) && this->s_AudioManager.m_CurrentMusicTitle != music)
						if (!this->s_AudioManager.replaceCurrentMusic(music))
							ImGui::InsertNotification({ ImGuiToastType_Warning, "Failed to load the music file!" });
				}
				ImGui::EndCombo();
			}
			if (ImGui::SliderFloat("Music volume: ", &this->s_AudioManager.m_musicvolume, 0.f, 100.f))
				this->s_AudioManager.m_CurrentMusic->setVolume(this->s_AudioManager.m_musicvolume);
			ImGui::SliderFloat("Game volume: ", &this->s_AudioManager.m_gamevolume, 0.f, 100.f);
			ImGui::SliderFloat("SFX volume: ", &this->s_AudioManager.m_sfxvolume, 0.f, 100.f);
		}
			break;
		case SETTINGS_STATE::INPUT:
			break;
		case SETTINGS_STATE::GAME:
			//ImGui::SliderFloat("Vertical Sensivity: ", &settings::m_VerticalSensivity, 0.f, 20.f);
			//ImGui::SliderFloat("Horizontal Sensivity: ", &settings::m_HorizontalSensivity, 0.f, 20.f);
			break;
		case SETTINGS_STATE::MAINSCREEN:
		{
			if (ImGui::SliderInt("Front image: ", &this->m_currFrontImage, 0, 0))
				this->m_FrontImage->Sprite.setTextureRect(sf::IntRect(sf::Vector2i(600 * this->m_currFrontImage, 0), sf::Vector2i(600, 600)));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				sf::Sprite background;
				background.setTexture(this->m_FrontImage->Texture);
				background.setTextureRect(sf::IntRect(sf::Vector2i(600 * this->m_currFrontImage, 0), sf::Vector2i(600, 600)));
				ImGui::Image(background, ImVec2(300.f, 300.f));
				ImGui::EndTooltip();
			}

			if (ImGui::SliderInt("Profile picture: ", &this->m_currIcon, 0, 3))
				this->m_Icon->Sprite.setTextureRect(sf::IntRect(sf::Vector2i(100 * this->m_currIcon, 0), sf::Vector2i(100, 100)));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				sf::Sprite background;
				background.setTexture(this->m_Icon->Texture);
				background.setTextureRect(sf::IntRect(sf::Vector2i(100 * this->m_currIcon, 0), sf::Vector2i(100, 100)));
				ImGui::Image(background, ImVec2(100.f, 100.f));
				ImGui::EndTooltip();
			}

			if (ImGui::SliderInt("Background image: ", &this->m_currBackgroundImage, 0, 3))
				this->m_BackgroundImage->Sprite.setTextureRect(sf::IntRect(sf::Vector2i(1920 * this->m_currBackgroundImage, 0), sf::Vector2i(1920, 1080)));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				sf::Sprite background;
				background.setTexture(this->m_BackgroundImage->Texture);
				background.setTextureRect(sf::IntRect(sf::Vector2i(1920 * this->m_currBackgroundImage, 0), sf::Vector2i(1920, 1080)));
				ImGui::Image(background, ImVec2(300.f, 300.f));
				ImGui::EndTooltip();
			}
		}
			break;
		default:
			break;
		}

		if (ImGui::Button("Save") && !this->s_Settings.save("Settings.ini"))
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Failed to save settings!"));
		ImGui::SameLine();
		if (ImGui::Button("Back & Save"))
		{
			if (!this->s_Settings.save("Settings.ini"))
				ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Failed to save settings!"));
			this->close();
		}
		ImGui::EndPopup();
	}

	if (!this->m_open)
		this->close();
	/*
	case settingStates::Input:
	{
		inputSystem is;
		static bool changeKeyBindings = false;
		static std::pair<std::string, Input>* toChange;
		for (auto& it : is.getInputHandler())
		{
			settingsPopUp.text(it.first.c_str());
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
					std::printf("...\n");
				if (ImGui::Selectable("Release"))
					std::printf("...\n");
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
	*/
}