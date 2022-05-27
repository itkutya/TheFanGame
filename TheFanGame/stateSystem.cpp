#include "stateSystem.h"

stateSystem::stateSystem() noexcept : m_add(false), m_replace(false), m_remove(false) {}

stateSystem::~stateSystem() noexcept {}

const void stateSystem::add(sf::RenderWindow& window, std::unique_ptr<state> toAdd, const bool& replace) noexcept
{
    this->m_add = true;
    this->m_newState = std::move(toAdd);

    this->m_replace = replace;
}

const void stateSystem::popCurrent() noexcept { m_remove = true; }

const void stateSystem::processStateChange(sf::RenderWindow& window) noexcept
{
    if (this->m_remove && (!this->m_stateStack.empty()))
    {
        this->m_stateStack.pop();
        this->m_remove = false;
    }

    if (this->m_add)
    {
        if (this->m_replace && (!this->m_stateStack.empty()))
        {
            this->m_stateStack.pop();
            this->m_replace = false;
        }

        this->m_stateStack.push(std::move(this->m_newState));
        this->m_stateStack.top()->init(window);
        this->m_add = false;
    }
}

const std::unique_ptr<state>& stateSystem::getCurrentState() const noexcept { return this->m_stateStack.top(); }

const std::size_t stateSystem::getCurrentSize() const noexcept { return this->m_stateStack.size(); }