#include "Tutorial.h"

void Tutorial::init(sf::RenderWindow& window)
{
	this->m_dungeon = std::make_unique<Dungeon>(std::initializer_list<Floor>{ Floor(5, 5), Floor(12, 12), Floor(24, 24) });
}

void Tutorial::processEvent(sf::Event& event) noexcept
{
}

void Tutorial::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	if (ImGui::Button("<-") && this->m_dungeon->m_currentFloor > 0)
		--this->m_dungeon->m_currentFloor;
	ImGui::SameLine();
	ImGui::Text("Current floor number: %i", this->m_dungeon->m_currentFloor);
	ImGui::SameLine();
	if (ImGui::Button("->") && this->m_dungeon->m_currentFloor < this->m_dungeon->m_floors.size() - 1)
		++this->m_dungeon->m_currentFloor;
}

void Tutorial::draw(sf::RenderWindow& window) noexcept
{
	window.draw(*this->m_dungeon);
}