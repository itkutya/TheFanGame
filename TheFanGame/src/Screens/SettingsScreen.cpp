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
			/*
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
			*/
			break;
		}
		case SETTINGS_STATE::PROFILE:
			break;
		case SETTINGS_STATE::AUDIO:
			break;
		case SETTINGS_STATE::INPUT:
			break;
		case SETTINGS_STATE::GAME:
			break;
		case SETTINGS_STATE::MAINSCREEN:
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
	case settingStates::Game:
	{
		ImGui::SliderFloat("Vertical Sensivity: ", &settings::m_VerticalSensivity, 0.f, 20.f);
		ImGui::SliderFloat("Horizontal Sensivity: ", &settings::m_HorizontalSensivity, 0.f, 20.f);
		break;
	}
	case settingStates::Mainmenu:
	{
		if (ImGui::SliderInt("Front image: ", &settings::m_currFrontPicture, 0, 0))
			this->m_frontImage.setTextureRect(sf::IntRect(sf::Vector2i(600 * settings::m_currFrontPicture, 0), sf::Vector2i(600, 600)));
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			sf::Sprite background;
			background.setTexture(*this->m_frontImage.getTexture());
			background.setTextureRect(sf::IntRect(sf::Vector2i(600 * settings::m_currFrontPicture, 0),
				sf::Vector2i(600, 600)));
			ImGui::Image(background, ImVec2(300.f, 300.f));
			ImGui::EndTooltip();
		}

		if (ImGui::SliderInt("Profile picture: ", &settings::m_currProfilePicture, 0, 3))
			this->m_icon.setTextureRect(sf::IntRect(sf::Vector2i(100 * settings::m_currProfilePicture, 0),
				sf::Vector2i(100, 100)));
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			sf::Sprite background;
			background.setTexture(*this->m_icon.getTexture());
			background.setTextureRect(sf::IntRect(sf::Vector2i(100 * settings::m_currProfilePicture, 0),
				sf::Vector2i(100, 100)));
			ImGui::Image(background, ImVec2(100.f, 100.f));
			ImGui::EndTooltip();
		}

		if (ImGui::SliderInt("Background image: ", &settings::m_currBackgroundPicture, 0, 3))
			m_backgroundImage.setTextureRect(sf::IntRect(sf::Vector2i(1920 * settings::m_currBackgroundPicture, 0),
				sf::Vector2i(1920, 1080)));
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			sf::Sprite background;
			background.setTexture(*m_backgroundImage.getTexture());
			background.setTextureRect(sf::IntRect(sf::Vector2i(1920 * settings::m_currBackgroundPicture, 0),
				sf::Vector2i(1920, 1080)));
			ImGui::Image(background, ImVec2(300.f, 300.f));
			ImGui::EndTooltip();
		}
		break;
	}
	case settingStates::Audio:
	{
		settingsPopUp.text("Currently playing: ");
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
		{
			settingsPopUp.close();
			guistateSystem::popCurrent();
		}
	}
	*/
}