#include "window.h"

window::window() noexcept : isFullscreen(false), FPSLimit(60) {}

window::~window() noexcept
{
	std::size_t maxSize = this->m_states.getSize();
	for (std::size_t i = 0; i < maxSize; ++i)
		this->m_states.popCurrent();
}

const void window::pollEvents() noexcept
{
	sf::Event event;
	while (this->m_window.pollEvent(event))
	{
		this->m_states.getState()->processEvent(event);

		if (event.type == sf::Event::Closed)
			this->m_window.close();

		if (event.type == sf::Event::Resized)
			this->onResize();

		if (inputManager::input(inputManager::m_Action["ScreenShot"], &event))
			this->ScreenShot();
	}
}

const void window::draw() noexcept
{
	this->m_window.clear();
	this->m_states.getState()->draw(this->m_window);
	this->m_window.display();
}

const void window::update() noexcept { this->m_states.getState()->update(this->m_window, this->deltaTime.restart()); }

const void window::onResize() noexcept 
{
	std::cout << "Window resized, new size is: " << this->m_window.getSize().x << "x" << this->m_window.getSize().y << '\n';
}

const void window::ScreenShot() noexcept
{
	sf::Texture texture;
	texture.create(this->m_window.getSize().x, this->m_window.getSize().y);
	texture.update(this->m_window);

	std::time_t t = std::time(0);
	std::tm now;
	localtime_s(&now, &t);
	std::ostringstream oss;
	oss << std::put_time(&now, "%H-%M-%S %d.%m.%Y.");
	auto str = oss.str();

	if (!texture.copyToImage().saveToFile("screenshots/" + str + ".png"))
		std::cout << "Error cannot create a screenshot...\n";
}

window::operator const bool() noexcept { return this->m_window.isOpen(); }

window::operator sf::RenderWindow&() noexcept { return this->m_window; }

const void window::create(const sf::VideoMode& size, const char* name) noexcept
{
	this->m_videomode = size;
	this->title = name;
	this->m_window.create(size, name);
	this->m_window.setKeyRepeatEnabled(false);
	this->deltaTime.restart();
}

const void window::setFramerateLimit(const std::uint32_t& limit) noexcept 
{
	this->FPSLimit = limit;
	this->m_window.setFramerateLimit(limit);
}

const void window::processStateChange(sf::RenderWindow& window) noexcept { this->m_states.processStateChange(window); }

const void window::popCurrent() { this->m_states.popCurrent(); }

const void window::setSize(const sf::Vector2u& size) noexcept 
{
	this->m_videomode = sf::VideoMode(size.x, size.y);
	this->recreate();
}

const void window::setFullscreen(const bool& active) noexcept
{
	this->isFullscreen = active;
	this->recreate();
}

const void window::recreate() noexcept
{
	this->m_window.setKeyRepeatEnabled(false);

	if (this->isFullscreen)
		this->m_window.create(this->m_videomode, this->title, sf::Style::Fullscreen);
	else
		this->m_window.create(this->m_videomode, this->title);

	this->setFramerateLimit(this->FPSLimit);
}