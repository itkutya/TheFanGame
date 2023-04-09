#include "ShopScreen.h"

void ShopScreen::init(sf::RenderWindow& window)
{
	this->m_open = true;
}

void ShopScreen::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	if (this->m_once)
	{
		ImGui::OpenPopup("Shop");
		this->m_once = false;
	}

	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Shop", &this->m_open, flags))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Sales"))
				this->m_state = SHOP_STATE::SALES;
			if (ImGui::Button("Characters"))
				this->m_state = SHOP_STATE::CHARACTERS;
			if (ImGui::Button("Icons"))
				this->m_state = SHOP_STATE::ICONS;
			if (ImGui::Button("Background Images"))
				this->m_state = SHOP_STATE::BACKGROUND;
			if (ImGui::Button("Other Stuff..."))
				this->m_state = SHOP_STATE::OTHERSTUFF;
			ImGui::EndMenuBar();
		}
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);

		switch (this->m_state)
		{
		case SHOP_STATE::SALES:
			break;
		case SHOP_STATE::CHARACTERS:
		{
			if (ImGui::BeginChild("##CharSet", ImVec2(600.f, 300.f), true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar))
			{
				/*
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
						if (CharactersPanel.button("Unlock", ImVec2(200.f, 50.f)) && profile::currency > this->characters[i].price)
						{
							profile::currency -= this->characters[i].price;
							this->characters[i].unlocked = true;
						}
						ImGui::PopID();
						if (this->s_Account.m_currency < this->characters[i].price)
							CharactersPanel.tooltip("Not enough currency, come back later.\nAnd put some usefull information here later...", ImVec4(1.f, 0.f, 0.f, 1.f));
						else
							CharactersPanel.tooltip(reinterpret_cast<const char*>(u8"This item costs 690$."), ImVec4(0.f, 1.f, 0.4f, 1.f));
					}
				}
				*/
				ImGui::EndChild();
			}

			if (ImGui::Button("Open loot box", ImVec2(300.f, 75.f)) && this->s_Account.m_currency >= 420)
				this->s_Account.m_currency -= 420;

			if (this->s_Account.m_currency < 420)
				ImGui::SetTooltip("Not enough currency, come back later.\nAnd put some usefull information here later...");
			else
				ImGui::SetTooltip("This item costs 420c.");
		}
			break;
		case SHOP_STATE::ICONS:
			break;
		case SHOP_STATE::BACKGROUND:
			break;
		case SHOP_STATE::OTHERSTUFF:
			break;
		}

		if (ImGui::Button("Back"))
			this->close();
		ImGui::EndPopup();
	}
	if (!this->m_open)
		this->close();
}