#include "window.h"

window::window(const sf::VideoMode& size, const char* name) noexcept 
{
	this->m_window.create(size, name);
	this->deltaTime.restart();

	this->MENU.init(this->m_window);

	quad[0].position = sf::Vector2f(30.f, 30.f);
	quad[1].position = sf::Vector2f(90.f, 30.f);
	quad[2].position = sf::Vector2f(90.f, 90.f);
	quad[3].position = sf::Vector2f(30.f, 90.f);

	quad[0].texCoords = sf::Vector2f(0, 0);
	quad[1].texCoords = sf::Vector2f(64, 0);
	quad[2].texCoords = sf::Vector2f(64, 64);
	quad[3].texCoords = sf::Vector2f(0, 64);
	this->addToDrawPool(quad);

	this->addToDrawPool(&c);
}

window::~window() noexcept {}

window::operator const bool() { return this->m_window.isOpen(); }

const void window::setFramerateLimit(const unsigned int& limit) { this->m_window.setFramerateLimit(limit); }

const void window::pollEvents() 
{
	sf::Event event;
	while (this->m_window.pollEvent(event))
	{
		this->MENU.processEvent(event);

		if (event.type == sf::Event::Closed)
			this->m_window.close();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->m_window.hasFocus())
			this->m_window.close();
	}
}

const void window::draw() 
{
	this->m_window.clear(sf::Color::Blue);

	for (auto& i : objects)
		this->m_window.draw(*i);

	this->MENU.render(this->m_window);

	this->m_window.display();
}

const void window::update() 
{
	this->MENU.update(this->m_window, this->deltaTime.restart());

	ImGui::Begin("Teszt");
	ImGui::Text("Hello world!");
	if (ImGui::Button("Hello"))
		std::cout << "asd\n";
	ImGui::Checkbox("Color", &e);
	if (e)
		this->quad[0].color = sf::Color::Red;
	else
		this->quad[0].color = sf::Color::Green;
	ImGui::SliderFloat("Radious", &this->r, 1.f, 30.f);
	this->c.setRadius(this->r);
	ImGui::End();
}

const void window::addToDrawPool(const sf::Drawable* object) { this->objects.emplace_back(object); }
