#include "menu.h"

menu::menu() noexcept
{
	resourceSystem::add<sf::Music>("Blackbird - Cecile Corbel", "res/MainMenu/Blackbird - Cecile Corbel.wav");
	resourceSystem::add<sf::Music>("Sakakibara Yui - Koi no Honoo", "res/MainMenu/Sakakibara Yui - Koi no Honoo.wav");
	resourceSystem::add<sf::Texture>("WallTexture", "res/wolftextures.png");
	resourceSystem::add<sf::Texture>("Background", "res/MainMenu/Backgrounds.png");
	resourceSystem::add<sf::Texture>("FrontImage", "res/MainMenu/FontImages.png");
	resourceSystem::add<sf::Texture>("Icon", "res/MainMenu/Icons.png");
	resourceSystem::add<sf::Texture>("Pause", "res/MainMenu/Pause.png");
	resourceSystem::add<sf::Texture>("Resume", "res/MainMenu/Resume.png");
	resourceSystem::add<sf::Texture>("CharacterTexture", "res/char.png");
	resourceSystem::add<sf::Font>("JP_Font", "res/Gen Jyuu Gothic Monospace Bold.ttf");
	resourceSystem::wait();
}

menu::~menu() noexcept 
{
	ImGui::SFML::Shutdown();
}

const void menu::init(window& window)
{
	ImGui::SFML::Init(window.getWindow());
	gui::loadStyle();
	gui::loadFont();

	settingsPanel::m_backgroundImage.setTexture(resourceSystem::c_get<sf::Texture>("Background"));
	settingsPanel::m_backgroundImage.setTextureRect(sf::IntRect({ 1920 * settings::m_currBackgroundPicture, 0 }, { 1920, 1080 }));
	settingsPanel::m_backgroundImage.setColor(sf::Color(255, 255, 255, 200));
	settingsPanel::m_backgroundImage.setScale(sf::Vector2f(window.getView().getSize().x / settingsPanel::m_backgroundImage.getLocalBounds().width,
														   window.getView().getSize().y / settingsPanel::m_backgroundImage.getLocalBounds().height));

	this->m_MainMusic = resourceSystem::get<std::unique_ptr<sf::Music>>(settings::m_currMusic).get();
	this->m_MainMusic->setLoop(true);
	if (settings::m_MusicVolume > 0.f)
		this->m_MainMusic->play();
	this->m_MainMusic->setVolume(settings::m_MusicVolume);

	if (settings::rememberToStayLogedIn)
		guistateSystem::add<mainmenuPanel>();
	else
		guistateSystem::add<loginPanel>();
}

const void menu::update(window& window, const sf::Time& dt) noexcept
{
	guistateSystem::processStateChange(window);

	ImGui::SFML::Update(window.getWindow(), dt);
	for (auto& state : guistateSystem::getAllStates())
		state->update(window, dt);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
	ImGui::RenderNotifications();
	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(1);
}

const void menu::processEvent(const sf::Event& event) noexcept 
{
	ImGui::SFML::ProcessEvent(event);
	for (auto& state : guistateSystem::getAllStates())
		state->processEvent(event);

	if (event.type == sf::Event::Resized)
	{
		settingsPanel::m_backgroundImage.setScale(sf::Vector2f((float)event.size.width / settingsPanel::m_backgroundImage.getLocalBounds().width,
													(float)event.size.height / settingsPanel::m_backgroundImage.getLocalBounds().height));
	}
}

const void menu::draw(window& window) noexcept 
{
	window.getWindow().setView(window.getView());
	window.getWindow().draw(settingsPanel::m_backgroundImage);
	window.getWindow().setView(window.getWindow().getDefaultView());

	for (auto& state : guistateSystem::getAllStates())
		state->draw(window);

	ImGui::SFML::Render(window.getWindow());
}