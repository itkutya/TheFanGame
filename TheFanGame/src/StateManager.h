#pragma once

#include <memory>
#include <stack>

#include "SFML/Graphics.hpp"

class State
{
public:
    explicit State() noexcept = default;
    virtual ~State() noexcept = default;
    virtual void init(sf::RenderWindow& window) = 0;
    virtual void processEvent(const sf::Event& event) noexcept = 0;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept = 0;
    virtual void draw(sf::RenderWindow& window) noexcept = 0;
};

class StateManager
{
public:
    StateManager(StateManager const&) = delete;
    void operator=(StateManager const&) = delete;
    virtual ~StateManager() noexcept = default;

    static StateManager& getInstance();

    template<typename T> void add(const bool& replace = false) noexcept;
    void popCurrent() noexcept;
    void processStateChange(sf::RenderWindow& window) noexcept;
    const std::unique_ptr<State>& getCurrentState() const noexcept;
    const std::size_t getSize() const noexcept;
private:
    explicit StateManager() noexcept = default;

    std::stack<std::unique_ptr<State>> m_statestack;
    std::unique_ptr<State> m_newstate;

    bool m_add = false;
    bool m_replace = false;
    bool m_remove = false;
};

template<typename T>
inline void StateManager::add(const bool& replace) noexcept
{
    this->m_add = true;
    this->m_newstate = std::move(std::make_unique<T>());
    this->m_replace = replace;
}