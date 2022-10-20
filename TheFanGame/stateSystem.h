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

class resourceSystem;
class inputSystem;
class stateSystem;
class settings;
class window;

struct engine
{
    resourceSystem* const Resources;
    inputSystem* const Inputs;
    stateSystem* const States;
    settings* const Settings;

    explicit engine(resourceSystem* rs, inputSystem* is, stateSystem* ss, settings* s)
        : Resources(rs), Inputs(is), States(ss), Settings(s) {};
    virtual ~engine() = default;
};

class state
{
public:
    inline state() noexcept = default;
    inline virtual ~state() noexcept = default;
    virtual const void init(sf::RenderWindow& window) = 0;
    virtual const void processEvent(const sf::Event& event) noexcept = 0;
    virtual const void update(sf::RenderWindow& window, const sf::Time& dt) noexcept = 0;
    virtual const void draw(sf::RenderWindow& window) noexcept = 0;
};

class stateSystem
{
public:
	stateSystem() = default;
	stateSystem(const stateSystem&) = delete;
	stateSystem(const stateSystem&&) = delete;
    stateSystem& operator=(stateSystem& other) = delete;
    stateSystem& operator=(const stateSystem& other) = delete;
	virtual ~stateSystem() noexcept 
    {
        std::size_t maxSize = getSize();
        for (std::size_t i = 0; i < maxSize; ++i)
            if ((!this->m_stateStack.empty()))
                this->m_stateStack.pop();
    };
    
    template<typename T>
    inline const void add(engine& e, window& w, const bool& replace = false) noexcept
    {
        this->m_add = true;
        this->m_newState = std::move(std::make_unique<T>(e, w));
        this->m_replace = replace;
    };
    
    const void popCurrent() noexcept;
    const void processStateChange(sf::RenderWindow& window) noexcept;
    const std::unique_ptr<state>& getState() noexcept;
    const std::size_t getSize() noexcept;
private:
    std::stack<std::unique_ptr<state>> m_stateStack;
    std::unique_ptr<state> m_newState;

    bool m_add = false;
    bool m_replace = false;
    bool m_remove = false;
};