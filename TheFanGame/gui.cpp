#include "gui.h"

gui::gui(window& window) noexcept { this->m_window = &window; }

gui::~gui() noexcept { ImGui::SFML::Shutdown(); }

const void gui::init(sf::RenderWindow& window)
{
	ImGui::SFML::Init(window);
	ImGui::StyleColorsDark();
}

const void gui::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	ImGui::SFML::Update(window, dt);

	if (ImGui::Begin("Main Window", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		ImGui::SetWindowSize("Main Window", ImVec2((float)window.getSize().x, (float)window.getSize().y));
		ImGui::SetWindowPos("Main Window", ImVec2(0.f, 0.f));

		ImGui::Text("FPS: %f", 1.f / dt.asSeconds());
		this->createImage(this->m_window->getTexture(0), sf::IntRect(0, 0, 64, 64), sf::Vector2f(350.f, 175.f), sf::Vector2f(300.f, 300.f));
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("This is a test tip!");

		ImGui::SetCursorPos(ImVec2(100.f, 65.f));
		ImGui::SliderFloat("Level: ", &this->xp, 0.f, 100.f, "%.3f", ImGuiSliderFlags_NoInput);

		if (this->createButton("Play", sf::Vector2f(50.f, 115.f), sf::Vector2f(200.f, 50.f)))
			this->m_window->addState<game>();
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Starts the game state.");

		if (this->createButton("Settings", sf::Vector2f(50.f, 215.f), sf::Vector2f(200.f, 50.f)))
			this->settingsPanel = true;
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Opens the setting menu.");

		if (this->createButton("Quit", sf::Vector2f(50.f, 315.f), sf::Vector2f(200.f, 50.f)))
			window.close();
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Goodbye!");

		if (this->settingsPanel)
		{
			if (ImGui::Begin("Settings Panel", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::SetWindowSize("Settings Panel", ImVec2((float)window.getSize().x / 1.2f, (float)window.getSize().y / 1.2f));
				ImGui::SetWindowPos("Settings Panel", ImVec2(25.f, 25.f));
				ImGui::SetWindowFocus("Settings Panel");

				if (this->createButton("Back", sf::Vector2f(ImGui::GetWindowSize().x - 250.f, ImGui::GetWindowSize().y - 100.f), sf::Vector2f(200.f, 50.f)))
					this->settingsPanel = false;

				ImGui::End();
			}
		}
		ImGui::End();
	}
}

const void gui::processEvent(const sf::Event& event) noexcept { ImGui::SFML::ProcessEvent(event); }

const void gui::draw(sf::RenderWindow& window) noexcept { ImGui::SFML::Render(window); }

const bool gui::createButton(const char* name, const sf::Vector2f& pos, const sf::Vector2f& size)
{
	ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
	if (ImGui::Button(name, ImVec2(size.x, size.y)))
		return true;
	return false;
}

const void gui::createImage(const sf::Texture& texture, const sf::IntRect& rect, const sf::Vector2f& pos, const sf::Vector2f& size)
{
	this->sprite.setTexture(texture);
	this->sprite.setTextureRect(rect);
	ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
	ImGui::Image(this->sprite, size);
}
