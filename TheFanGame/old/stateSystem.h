#pragma once

#include "includes.h"

class window;

class state
{
public:
    inline state() noexcept = default;
    inline virtual ~state() noexcept = default;
    virtual const void init(window& window) = 0;
    virtual const void processEvent(const sf::Event& event) noexcept = 0;
    virtual const void update(window& window, const sf::Time& dt) noexcept = 0;
    virtual const void draw(window& window) noexcept = 0;
};

class stateSystem
{
public:
	stateSystem() = delete;
	stateSystem(const stateSystem&) = delete;
	stateSystem(const stateSystem&&) = delete;
    stateSystem& operator=(stateSystem& other) = delete;
    stateSystem& operator=(const stateSystem& other) = delete;
	virtual ~stateSystem() noexcept;
    
    template<typename T>
    static inline const void add(const bool& replace = false) noexcept
    {
        m_add = true;
        m_newState = std::move(std::make_unique<T>());
        m_replace = replace;
    };

    static const void cleanUp() noexcept;
    static const void popCurrent() noexcept;
    static const void processStateChange(window& window) noexcept;
    static const std::unique_ptr<state>& getState() noexcept;
    static const std::size_t getSize() noexcept;
private:
    static inline std::stack<std::unique_ptr<state>> m_stateStack;
    static inline std::unique_ptr<state> m_newState;

    static inline bool m_add = false;
    static inline bool m_replace = false;
    static inline bool m_remove = false;
};