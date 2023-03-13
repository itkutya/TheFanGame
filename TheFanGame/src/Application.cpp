#include "Application.h"

Application::Application(sf::VideoMode size, const std::string& title, bool fullscreen, std::uint32_t fpslimit) noexcept
{
	this->m_size = size;
	this->m_title = title;
	this->m_fullscreen = fullscreen;
	this->m_fpslimit = fpslimit;
	this->m_window.create(size, title, fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	this->m_window.setFramerateLimit(fpslimit);

	this->m_videomodes = sf::VideoMode::getFullscreenModes();

	ImGui::SFML::Init(this->m_window);
	this->s_StateManager.add<Menu>(this);

	this->m_deltatime.restart();
}

Application::~Application() noexcept
{
	ImGui::SFML::Shutdown();
}

sf::RenderWindow& Application::getWindow() noexcept
{
	return this->m_window;
}

void Application::pollEvents() noexcept
{
	this->s_StateManager.processStateChange(this->m_window);

	sf::Event event;
	while (this->m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed)
			this->m_window.close();

		if (event.type == sf::Event::Resized)
			this->m_size = sf::VideoMode({ event.size.width, event.size.height });

		if (this->s_StateManager.getSize())
			this->s_StateManager.getCurrentState()->processEvent(event);
	}
}

void Application::update() noexcept
{
	const sf::Time dt = this->m_deltatime.restart();
	ImGui::SFML::Update(this->m_window, dt);
	if (this->s_StateManager.getSize())
		this->s_StateManager.getCurrentState()->update(this->m_window, dt);
}

void Application::draw() noexcept
{
	this->m_window.clear();
	if (this->s_StateManager.getSize())
		this->s_StateManager.getCurrentState()->draw(this->m_window);
	ImGui::SFML::Render(this->m_window);
	this->m_window.display();
}

void Application::recreateWindow()
{
	this->m_window.create(this->m_size , this->m_title, this->m_fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	this->m_window.setFramerateLimit(this->m_fpslimit);
}