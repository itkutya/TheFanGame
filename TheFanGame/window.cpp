#include "window.h"

window::window(const sf::VideoMode& size, const char* name) : videoMode(size), title(name)
{
	this->m_window.create(videoMode, title);

	this->m_event = sf::Event();
}

window::~window()
{

}

window::operator bool() const
{
	return m_window.isOpen();
}

const void window::pollEvents()
{
	while (this->m_window.pollEvent(this->m_event))
	{
		if (this->m_event.type == sf::Event::Closed)
			this->m_window.close();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			this->m_window.close();
	}
}
