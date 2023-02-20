#include "charactersPanel.h"

charactersPanel::charactersPanel() noexcept
{
	//Load characters...
	for (std::size_t i = 0; i < 5; ++i)
		this->characters.emplace_back();
}

const void charactersPanel::init(window& window) noexcept
{
}

const void charactersPanel::processEvent(const sf::Event& event) noexcept
{
}

const void charactersPanel::update(window& window, const sf::Time& dt) noexcept
{
	popup CharactersPanel("Characters", window.getWindow());
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
				if (CharactersPanel.button("Unlock", ImVec2(200.f, 50.f)) && profile::currency > this->characters[i].price)
				{
					profile::currency -= this->characters[i].price;
					this->characters[i].unlocked = true;
				}
				ImGui::PopID();
				if (profile::currency < this->characters[i].price)
					CharactersPanel.tooltip("Not enough currency, come back later.\nAnd put some usefull information here later...", ImVec4(1.f, 0.f, 0.f, 1.f));
				else
					CharactersPanel.tooltip(reinterpret_cast<const char*>(u8"This item costs 690$."), ImVec4(0.f, 1.f, 0.4f, 1.f));
			}
		}
		ImGui::EndChild();
	}

	if (CharactersPanel.button("Open loot box", ImVec2(300.f, 75.f)) && profile::currency >= 420)
		profile::currency -= 420;

	if (profile::currency < 420)
		CharactersPanel.tooltip("Not enough currency, come back later.\nAnd put some usefull information here later...", ImVec4(1.f, 0.f, 0.f, 1.f));
	else
		CharactersPanel.tooltip("This item costs 420c.", ImVec4(0.f, 1.f, 0.4f, 1.f));

	if (ImGui::Button("Back##Characters", ImVec2(300.f, 75.f)))
	{
		CharactersPanel.close();
		guistateSystem::popCurrent();
	}
}

const void charactersPanel::draw(window& window) noexcept
{
	return void();
}
