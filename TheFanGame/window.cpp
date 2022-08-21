#include "window.h"

window::window(const char* name) noexcept : title(name)
{
	if (!settings::loadSettings("res/Settings.ini"))
		std::cout << "NOT_OK!\n";

	this->recreate();
	this->deltaTime.restart();
}

window::~window() noexcept 
{
	if (!settings::saveSettings("res/Settings.ini"))
		std::cout << "NOT_OK!\n";
}

const void window::pollEvents() noexcept
{
	sf::Event event;
	while (this->m_window.pollEvent(event))
	{
		stateSystem::getState()->processEvent(event);

		if (event.type == sf::Event::Closed)
			this->m_window.close();

		if (inputSystem::input(inputSystem::m_Action["ScreenShot"], &event))
			this->ScreenShot();
	}
}

const void window::draw() noexcept
{
	this->m_window.clear();
	stateSystem::getState()->draw(this->m_window);
	this->m_window.display();
}

const void window::update() noexcept { stateSystem::getState()->update(this->m_window, this->deltaTime.restart()); }

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

const void window::setFramerateLimit() noexcept
{
	if (!settings::m_Vsync)
		if (!settings::m_isFPSLimited)
			this->m_window.setFramerateLimit(0);
		else
			this->m_window.setFramerateLimit(settings::m_FpsLimit);
}

const void window::setVSync() noexcept
{
	if (!settings::m_isFPSLimited)
		this->m_window.setVerticalSyncEnabled(settings::m_Vsync);
}

const void window::recreate() noexcept
{
	if (settings::m_Fullscreen)
		this->m_window.create(settings::m_Videomodes[settings::m_currVideomode], this->title, sf::Style::Fullscreen);
	else
		this->m_window.create(settings::m_Videomodes[settings::m_currVideomode], this->title);

	this->m_window.setKeyRepeatEnabled(false);

	if (!settings::m_Vsync)
		if (!settings::m_isFPSLimited)
			this->m_window.setFramerateLimit(0);
		else
			this->m_window.setFramerateLimit(settings::m_FpsLimit);
	else
		std::cout << "Disable Vsync before settings a FPS limit...\n";

	if (!settings::m_isFPSLimited)
		this->m_window.setVerticalSyncEnabled(true);
	else
		this->m_window.setVerticalSyncEnabled(false);
}