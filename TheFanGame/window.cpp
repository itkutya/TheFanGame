#include "window.h"

window::window(const sf::VideoMode& size, const char* name, context* context) noexcept
{
	this->m_window.create(size, name);
	this->deltaTime.restart();

	this->m_context = context;
	this->m_context->g_states.add(this->m_window, std::make_unique<gui>(this->m_context));
}

window::~window() noexcept 
{
	std::size_t maxSize = this->m_context->g_states.getCurrentSize();
	for (std::size_t i = 0; i < maxSize; ++i)
		this->m_context->g_states.popCurrent();
}

const void window::pollEvents() noexcept
{
	sf::Event event;
	while (this->m_window.pollEvent(event))
	{
		if (this->m_context->g_states.getCurrentSize() > 0)
			this->m_context->g_states.getCurrentState()->processEvent(event);

		if (event.type == sf::Event::Closed)
			this->m_window.close();
	}
}

const void window::draw() noexcept
{
	this->m_window.clear(sf::Color::Blue);
	if (this->m_context->g_states.getCurrentSize() > 0)
		this->m_context->g_states.getCurrentState()->draw(this->m_window);
	this->m_window.display();
}

const void window::update() noexcept
{
	if (this->m_context->g_states.getCurrentSize() > 0)
		this->m_context->g_states.getCurrentState()->update(this->m_window, this->deltaTime.restart());
}

sf::RenderWindow& window::getWindow() noexcept { return this->m_window; }

window::operator const bool() noexcept { return this->m_window.isOpen(); }

const void window::setFramerateLimit(const unsigned int& limit) noexcept { this->m_window.setFramerateLimit(limit); }

const void window::addToDrawPool(const sf::Drawable* object) noexcept { this->objects.emplace_back(object); }
