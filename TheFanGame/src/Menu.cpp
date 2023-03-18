#include "Menu.h"

void Menu::init(sf::RenderWindow& window)
{
	ImGui::LoadCostumeFont("Resources/default.ttf");
	ImGui::LoadCostumeStyle();

	this->m_BackgroundImage = this->s_ResourceManager.add<ResourceManager::Object>("BackgroundImage");
	if (this->m_BackgroundImage->Texture.loadFromFile("Resources/Backgrounds.png"))
	{
		this->m_BackgroundImage->Sprite.setTexture(this->m_BackgroundImage->Texture);
		this->m_BackgroundImage->Sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 1920, 1080 }));
		this->m_BackgroundImage->Sprite.setPosition(sf::Vector2f({ 0.f, 0.f }));
	}

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
	window.draw(this->m_BackgroundImage->Sprite);

	for (auto& state : this->s_StateManager.getCurrentGUIState())
		state->draw(window);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
	ImGui::RenderNotifications();
	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(1);
}