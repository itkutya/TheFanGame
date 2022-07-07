#include "window.h"

window::window() noexcept : m_context(nullptr) {}

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

const void window::onResize() noexcept {}

window::operator const bool() noexcept { return this->m_window.isOpen(); }

window::operator sf::RenderWindow&() noexcept { return this->m_window; }

const void window::create(const sf::VideoMode& size, const char* name) noexcept
{
	this->m_window.create(size, name);
	this->deltaTime.restart();
}

const void window::setFramerateLimit(const unsigned int& limit) noexcept { this->m_window.setFramerateLimit(limit); }

const void window::setContext(context& context) noexcept { this->m_context = &context; }

const void window::processStateChange(sf::RenderWindow& window) noexcept { this->m_context->m_states.processStateChange(window); }

const void window::popCurrent() { this->m_context->m_states.popCurrent(); }

const sf::Texture& window::getTexture(const int& index) const { return this->m_context->m_resources.getTexture(index); }