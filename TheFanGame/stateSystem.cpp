#include "stateSystem.h"

stateSystem::stateSystem() noexcept : m_replace(false) {}

stateSystem::~stateSystem() noexcept {}

const void stateSystem::add(sf::RenderWindow& window, std::unique_ptr<state> toAdd, const bool& replace) noexcept
{
    this->m_newState = std::move(toAdd);
    this->m_replace = replace;

    if (this->m_replace && (!this->m_stateStack.empty()))
        this->m_stateStack.pop();

    this->m_stateStack.push(std::move(this->m_newState));
    this->m_stateStack.top()->init(window);
}

const void stateSystem::popCurrent() noexcept 
{
    if ((!this->m_stateStack.empty()))
        this->m_stateStack.pop();
}

const std::unique_ptr<state>& stateSystem::getCurrentState() const noexcept { return this->m_stateStack.top(); }

const std::size_t stateSystem::getCurrentSize() const noexcept { return this->m_stateStack.size(); }