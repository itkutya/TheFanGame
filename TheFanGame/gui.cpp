#include "gui.h"

gui::gui(std::unique_ptr<stateSystem>& context) noexcept 
{ 
	m_resources = std::make_shared<resourceManager>(); 
	this->m_context = context.get();
}

gui::~gui() noexcept { ImGui::SFML::Shutdown(); m_resources.reset(); }

const void gui::init(sf::RenderWindow& window)
{
	m_resources->addTexture(0, "res/wolftextures.png");
	ImGui::SFML::Init(window);
	ImGui::StyleColorsDark();

	this->xp = 0.f;
	this->sprite.setTexture(m_resources->getTexture(0));
}

const void gui::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
	ImGui::SFML::Update(window, dt);
	ImGui::Begin("Main Window", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize(ImVec2((float)window.getSize().x, (float)window.getSize().y));
	ImGui::SetWindowPos(ImVec2(0.f, 0.f));

	ImGui::Text("FPS: %f", 1.f / dt.asSeconds());
	ImGui::SetCursorPos(ImVec2(25.f, 45.f));
	this->sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	ImGui::Image(this->sprite, sf::Vector2f(50.f, 50.f));
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("This is a test tip!");
	ImGui::SetCursorPos(ImVec2(300.f, 115.f));
	this->sprite.setTextureRect(sf::IntRect(64, 0, 64, 64));
	ImGui::Image(this->sprite, sf::Vector2f(200.f, 200.f));
	ImGui::SetCursorPos(ImVec2(100.f, 65.f));
	ImGui::SliderFloat("Level: ", &this->xp, 0.f, 100.f, "%.3f", ImGuiSliderFlags_NoInput);
	ImGui::SetCursorPos(ImVec2(50.f, 115.f));
	if (ImGui::Button("Play", ImVec2(200.f, 50.f)))
		this->m_context->add(window, std::make_unique<game>());
	ImGui::SetCursorPos(ImVec2(50.f, 215.f));
	if (ImGui::Button("Settings", ImVec2(200.f, 50.f)))
		std::cout << "Settings\n";
	ImGui::SetCursorPos(ImVec2(50.f, 315.f));
	if (ImGui::Button("Quit", ImVec2(200.f, 50.f)))
		window.close();

	ImGui::End();
}

const void gui::processEvent(const sf::Event& event) noexcept { ImGui::SFML::ProcessEvent(event); }

const void gui::draw(sf::RenderWindow& window) noexcept { ImGui::SFML::Render(window); }