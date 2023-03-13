#include "Menu.h"

void Menu::init(sf::RenderWindow& window)
{
	ImGui::LoadCostumeFont("Resources/default.ttf");
	ImGui::LoadCostumeStyle();

	this->s_StateManager.addGUIState<LoginScreen>(this->m_app);
}

void Menu::processEvent(sf::Event& event) noexcept
{
	for (auto& state : this->s_StateManager.getCurrentGUIState())
		state->processEvent(event);
}

void Menu::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	for (auto& state : this->s_StateManager.getCurrentGUIState())
		state->update(window, dt);
}

void Menu::draw(sf::RenderWindow& window) noexcept
{
	for (auto& state : this->s_StateManager.getCurrentGUIState())
		state->draw(window);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
	ImGui::RenderNotifications();
	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(1);
}