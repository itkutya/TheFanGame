#include "window.h"

window::window(const sf::VideoMode& size, const char* name) {
	this->m_window.create(size, name);
	this->m_event = sf::Event();
	this->deltaTime.restart();

	quad[0].position = sf::Vector2f(30.f, 30.f);
	quad[1].position = sf::Vector2f(90.f, 30.f);
	quad[2].position = sf::Vector2f(90.f, 90.f);
	quad[3].position = sf::Vector2f(30.f, 90.f);
	this->addToDrawPool(quad);
}

window::~window() {}

window::operator const bool() {
	this->pollEvents();
	return this->m_window.isOpen();
}

const void window::pollEvents() {
	while (this->m_window.pollEvent(this->m_event)) {
		if (this->m_event.type == sf::Event::Closed)
			this->m_window.close();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->m_window.hasFocus())
			this->m_window.close();
	}
}

const void window::draw() {
	this->m_window.clear(sf::Color::Black);
	for (auto& i : objects)
		this->m_window.draw(*i);
	this->m_window.display();
}

void window::update() {
	float dt = this->deltaTime.getElapsedTime().asSeconds();
	this->deltaTime.restart();
}