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
    state() noexcept {};
    virtual ~state() noexcept {};

    virtual const void init(sf::RenderWindow& window) = 0;
    virtual const void processEvent(const sf::Event& event) noexcept = 0;
    virtual const void update(sf::RenderWindow& window, const sf::Time& dt) noexcept = 0;
    virtual const void draw(sf::RenderWindow& window) noexcept = 0;
};

class stateSystem
{
public:
	stateSystem() noexcept;
	virtual ~stateSystem() noexcept;

    const void add(sf::RenderWindow& window, std::unique_ptr<state> toAdd, const bool& replace = false) noexcept;
    const void popCurrent() noexcept;
    const void ProcessStateChange(sf::RenderWindow& window) noexcept;
    const std::unique_ptr<state>& getCurrentState() const noexcept;
    const std::size_t getCurrentSize() const noexcept;
private:
    std::stack<std::unique_ptr<state>> m_stateStack;
    std::unique_ptr<state> m_newState;

    bool m_add;
    bool m_replace;
    bool m_remove;
};