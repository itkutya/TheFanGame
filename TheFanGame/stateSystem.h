#pragma once

#include <iostream>
#include <stack>
#include <memory>
#include <vector>

#if _WIN32 || _WIN64
    #if _WIN64
        #include "SFML64/Graphics.hpp"
    #else
        #include "SFML32/Graphics.hpp"
    #endif
#endif

#include "window.h"

class window;

class state
{
public:
    //Construct's the state
    inline state() noexcept {};
    //Destruct's the state
    inline virtual ~state() noexcept {};

    //Init the current state
    virtual const void init(sf::RenderWindow& window) = 0;
    //Process the current state events
    virtual const void processEvent(const sf::Event& event) noexcept = 0;
    //Update the current state
    virtual const void update(sf::RenderWindow& window, const sf::Time& dt) noexcept = 0;
    //Draw the current state objects
    virtual const void draw(sf::RenderWindow& window) noexcept = 0;
};

class stateSystem
{
public:
	stateSystem() = delete;
	stateSystem(const stateSystem&) = delete;
	stateSystem(const stateSystem&&) = delete;
	virtual ~stateSystem() noexcept {};

    //Add a new state and either replace it or add it to the top of the stack.
    template<typename T>
    inline static const void add(window& window, const bool& replace = false) noexcept
    {
        m_add = true;
        m_newState = std::move(std::make_unique<T>(window));
        m_replace = replace;
    };
    //Delete the top state.
    static const void popCurrent() noexcept;
    //Process the change's made in the state system in the begining of the main loop to prevent undifend error's.
    static const void processStateChange(sf::RenderWindow& window) noexcept;
    //Get the address of the current state.
    static const std::unique_ptr<state>& getState() noexcept;
    //Get the size of the stack.
    static const std::size_t getSize() noexcept;
    //Clears stack states.
    static const void clear();
private:
    static std::stack<std::unique_ptr<state>> m_stateStack;
    static std::unique_ptr<state> m_newState;

    static bool m_add;
    static bool m_replace;
    static bool m_remove;
};