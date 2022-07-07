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
    //Construct's the state system with everything set to false.
	stateSystem() noexcept;
    //Default destructor.
	virtual ~stateSystem() noexcept;

    //Add a new state and either replace it or add it to the top of the stack.
    const void add(sf::RenderWindow& window, std::unique_ptr<state> toAdd, const bool& replace = false) noexcept;
    //Delete the top state.
    const void popCurrent() noexcept;
    //Process the change's made in the state system in the begining of the main loop to prevent undifend error's.
    const void processStateChange(sf::RenderWindow& window) noexcept;
    //Get the address of the current state.
    const std::unique_ptr<state>& getState() const noexcept;
    //Get the size of the stack.
    const std::size_t getSize() const noexcept;
private:
    std::stack<std::unique_ptr<state>> m_stateStack;
    std::unique_ptr<state> m_newState;

    bool m_add;
    bool m_replace;
    bool m_remove;
};