#include "gui.h"

gui::gui() noexcept {}

gui::~gui() noexcept
{
	ImGui::SFML::Shutdown();
}

const void gui::init(sf::RenderWindow& window) noexcept
{
	ImGui::SFML::Init(window);
}

const void gui::processEvent(const sf::Event& event) noexcept
{
	ImGui::SFML::ProcessEvent(event);
}

const void gui::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	ImGui::SFML::Update(window, dt);
}

const void gui::render(sf::RenderWindow& window) noexcept
{
	ImGui::SFML::Render(window);
}
