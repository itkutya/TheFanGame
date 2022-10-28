#include "window.h"

window::window(const sf::VideoMode& vm, const std::string& name, const bool& fs, const std::uint32_t& limit, const bool& vsync, engine& e) noexcept 
	: m_enigne(e)
{
	this->create(vm, name, fs, limit, vsync);
	this->deltaTime.restart();
}

const void window::pollEvents() noexcept
{
	sf::Event event;
	while (this->m_window.pollEvent(event))
	{
		this->m_enigne.States->getState()->processEvent(event);

		if (event.type == sf::Event::Closed)
			this->m_window.close();

		if (this->m_enigne.Inputs->operator()("ScreenShot", &event))
			this->ScreenShot();
	}
}

const void window::draw() noexcept
{
	this->m_window.clear();
	this->m_enigne.States->getState()->draw(this->m_window);
	this->m_window.display();
}

const void window::update() noexcept { this->m_enigne.States->getState()->update(this->m_window, this->deltaTime.restart()); }

const void window::ScreenShot() noexcept
{
	sf::Texture texture;
	bool status = texture.create(sf::Vector2u(this->m_window.getSize().x, this->m_window.getSize().y));
	texture.update(this->m_window);

	std::time_t t = std::time(0);
	std::tm now;
	localtime_s(&now, &t);
	std::ostringstream oss;
	oss << std::put_time(&now, "%H-%M-%S %d.%m.%Y.");
	auto str = oss.str();

	if (!texture.copyToImage().saveToFile("screenshots/" + str + ".png"))
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error while taking a screenshot!"));
	else
		ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Success, 3000, "Yay! Your screenshot can be found at\nscreenshots/%s.png", str.c_str()));
}

window::operator const bool() const noexcept { return this->m_window.isOpen(); }

window::operator sf::RenderWindow&() noexcept { return this->m_window; }

const void window::setFramerateLimit(const std::uint32_t& limit) noexcept 
{
	if (limit > 0)
		this->m_window.setVerticalSyncEnabled(false);
	this->m_window.setFramerateLimit(limit);
}

const void window::setVSync(const bool& vsync) noexcept 
{
	if (vsync)
		this->m_window.setFramerateLimit(0);
	this->m_window.setVerticalSyncEnabled(vsync);
}

const void window::create(const sf::VideoMode& vm, const std::string& name, const bool& fs, const std::uint32_t& limit, const bool& vsync) noexcept
{
	fs ? this->m_window.create(vm, name, sf::Style::Fullscreen) : this->m_window.create(vm, name);
	limit ? setFramerateLimit(limit) : setVSync(vsync);
}