#include "window.h"

window::window() noexcept : m_context(nullptr), isFullscreen(false), FPSLimit(60) {}

window::~window() noexcept
{
	if (this->m_context != nullptr)
	{
		std::size_t maxSize = this->m_context->m_states.getSize();
		for (std::size_t i = 0; i < maxSize; ++i)
			this->m_context->m_states.popCurrent();
	}
}

const void window::pollEvents() noexcept
{
	sf::Event event;
	while (this->m_window.pollEvent(event))
	{
		if (this->m_context != nullptr)
			this->m_context->m_states.getState()->processEvent(event);

		if (event.type == sf::Event::Closed)
			this->m_window.close();

		if (event.type == sf::Event::Resized)
			this->onResize();
	}
}

const void window::draw() noexcept
{
	this->m_window.clear();
	if (this->m_context != nullptr)
		this->m_context->m_states.getState()->draw(this->m_window);
	this->m_window.display();
}

const void window::update() noexcept
{
	if (this->m_context != nullptr)
		this->m_context->m_states.getState()->update(this->m_window, this->deltaTime.restart());
}

const void window::onResize() noexcept 
{
	std::cout << "Window resized, new size is: " << this->m_window.getSize().x << "x" << this->m_window.getSize().y << '\n';
}

window::operator const bool() noexcept { return this->m_window.isOpen(); }

window::operator sf::RenderWindow&() noexcept { return this->m_window; }

const void window::create(const sf::VideoMode& size, const char* name) noexcept
{
	this->m_videomode = size;
	this->title = name;
	this->m_window.create(size, name);
	this->deltaTime.restart();
}

const void window::setFramerateLimit(const std::uint32_t& limit) noexcept 
{
	this->FPSLimit = limit;
	this->m_window.setFramerateLimit(limit);
}

const void window::setContext(context& context) noexcept { this->m_context = &context; }

const void window::processStateChange(sf::RenderWindow& window) noexcept { this->m_context->m_states.processStateChange(window); }

const void window::popCurrent() { this->m_context->m_states.popCurrent(); }

const sf::Texture& window::getTexture(const std::uint8_t& index) const { return this->m_context->m_resources.getTexture(index); }

const sf::Font& window::getFont(const std::uint8_t& index) const { return this->m_context->m_resources.getFont(index); }

const sf::SoundBuffer& window::getSoundBuffer(const std::uint8_t& index) const { return this->m_context->m_resources.getSoundBuffer(index); }

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
	if (this->isFullscreen)
		this->m_window.create(this->m_videomode, this->title, sf::Style::Fullscreen);
	else
		this->m_window.create(this->m_videomode, this->title);

	this->setFramerateLimit(this->FPSLimit);
}
