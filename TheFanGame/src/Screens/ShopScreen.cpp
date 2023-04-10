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
				for (auto& [name, character] : this->s_CharactersManager.m_Characters)
				{
					sf::Sprite temp;
					temp.setTexture(character.m_Icon);
					ImGui::Image(temp, sf::Vector2f(150.f, 150.f));

					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						sf::Sprite ability;
						ability.setTexture(character.m_Icon);
						ImGui::Image(ability, sf::Vector2f(50.f, 50.f));
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1.f, 0.0f, 0.1f, 1.f), "Health: %.1f %s\nATK: %u %s\nDEF: %u %s\nSpeed: %.1f %s\nPrice: %u %s\n",
							character.m_Health, ICON_FA_HEART, 
							character.m_ATK, ICON_FA_HAMMER, 
							character.m_DEF, ICON_FA_DEMOCRAT, 
							character.m_Speed, ICON_FA_SHOE_PRINTS, 
							character.m_Price, ICON_FA_MONEY_BILL);
						ImGui::EndTooltip();
					}
					ImGui::SameLine();
					ImGui::Text("%s", name.data());

					if (!character.m_Unlocked)
					{
						ImGui::PushID(name.data());
						if (ImGui::Button("Unlock", ImVec2(200.f, 50.f)) && this->s_Account.m_currency > character.m_Price)
						{
							this->s_Account.m_currency -= character.m_Price;
							character.m_Unlocked = true;
						}
						ImGui::PopID();
						if (ImGui::IsItemHovered() && this->s_Account.m_currency < character.m_Price)
							ImGui::SetTooltip("Not enough currency, come back later.\nAnd put some usefull information here later...");
						else if (ImGui::IsItemHovered())
							ImGui::SetTooltip(reinterpret_cast<const char*>(u8"This item costs 690$."));
					}
				}
				ImGui::EndChild();
			}
		}
			break;
		case SHOP_STATE::ICONS:
			break;
		case SHOP_STATE::BACKGROUND:
			break;
		case SHOP_STATE::OTHERSTUFF:
		{
			if (ImGui::Button("Open loot box", ImVec2(300.f, 75.f)) && this->s_Account.m_currency >= 420)
				this->s_Account.m_currency -= 420;
			if (ImGui::IsItemHovered() && this->s_Account.m_currency < 420)
				ImGui::SetTooltip("Not enough currency, come back later.\nAnd put some usefull information here later...");
			else if (ImGui::IsItemHovered())
				ImGui::SetTooltip("This item costs 420c.");
		}
			break;
		}

		if (ImGui::Button("Back"))
			this->close();
		ImGui::EndPopup();
	}
	if (!this->m_open)
		this->close();
}