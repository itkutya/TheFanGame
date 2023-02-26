#include "Menu.h"

void Menu::init(sf::RenderWindow& window)
{
	ImGui::LoadCostumeFont("Resources/default.ttf");
	ImGui::LoadCostumeStyle();

	this->s_StateManager->getCurrentGUIState().emplace_back(std::make_unique<Login>())->init(window);
}

void Menu::processEvent(const sf::Event& event) noexcept
{
	for (auto& state : this->s_StateManager->getCurrentGUIState())
		state->processEvent(event);
}

void Menu::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	for (auto& state : this->s_StateManager->getCurrentGUIState())
		state->update(window, dt);
}

void Menu::draw(sf::RenderWindow& window) noexcept
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
	ImGui::RenderNotifications();
	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(1);

	for (auto& state : this->s_StateManager->getCurrentGUIState())
		state->draw(window);
}