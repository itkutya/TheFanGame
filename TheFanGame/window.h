#pragma once

#include "stateSystem.h"
#include "resourceManager.h"

struct context
{
	stateSystem m_states;
	resourceManager m_resources;
};

class window
{
public:
	//Construct's the window class.
	window() noexcept;
	//Destruct's the window class.
	virtual ~window() noexcept;
	//Returns m_window.isOpen().
	explicit operator const bool() noexcept;
	//Returns the address of m_window.
	operator sf::RenderWindow&() noexcept;
	//Creates the renderwindow with the given size and name.
	const void create(const sf::VideoMode& size, const char* name) noexcept;
	//Add's a state to the state pool.
	template<typename T>
	inline const void addState(const bool& replace = false)
	{
		if (this->m_context != nullptr)
			this->m_context->m_states.add(this->m_window, std::make_unique<T>(*this), replace);
		else
			throw "No context was found...\n";
	}
	//Set's the framerate of the m_window.
	const void setFramerateLimit(const unsigned int& limit) noexcept;
	//Set's the context for the main loop.
	const void setContext(context& context) noexcept;
	//Processes the state changes.
	const void processStateChange(sf::RenderWindow& window) noexcept;
	//Delete current state.
	const void popCurrent();
	//Get the texture at the given index.
	const sf::Texture& getTexture(const int& index) const;
	//Poll events in the main loop.
	const void pollEvents() noexcept;
	//Draw's everything on the m_window.
	const void draw() noexcept;
	//Update's the game logic.
	const void update() noexcept;
private:
	sf::RenderWindow m_window;
	sf::Clock deltaTime;
	context* m_context;

	//On the m_window resize this function will triger.
	const void onResize() noexcept;
};