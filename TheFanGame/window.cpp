#include "window.h"

window::window(const sf::VideoMode& size, const char* name)
{
	this->m_window.create(size, name);

	this->m_event = sf::Event();
}

window::~window() {}

window::operator const bool()
{
	this->pollEvents();
	return this->m_window.isOpen();
}

const void window::pollEvents()
{
	while (this->m_window.pollEvent(this->m_event))
	{
		if (this->m_event.type == sf::Event::Closed)
			this->m_window.close();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->m_window.hasFocus())
			this->m_window.close();
	}
}

const void window::draw()
{
	this->m_window.clear(sf::Color::Black);
	for (auto& i : objects)
		this->m_window.draw(*i);
	this->m_window.display();
}

void window::update() 
{
}

const void window::setFramerateLimit(const unsigned int& limit)
{
	this->m_window.setFramerateLimit(limit);
}

void window::addToDrawPool(const sf::Drawable* object)
{
	this->objects.emplace_back(object);
}
