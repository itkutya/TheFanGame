#include "stateSystem.h"

stateSystem::stateSystem() noexcept : m_add(false), m_replace(false), m_remove(false) {}

stateSystem::~stateSystem() noexcept {}

const void stateSystem::add(sf::RenderWindow& window, std::unique_ptr<state> toAdd, const bool& replace) noexcept
{
    m_add = true;
    m_newState = std::move(toAdd);

    m_replace = replace;
}

const void stateSystem::popCurrent() noexcept { m_remove = true; }

const void stateSystem::ProcessStateChange(sf::RenderWindow& window) noexcept
{
    if (m_remove && (!m_stateStack.empty()))
    {
        m_stateStack.pop();

        m_remove = false;
    }

    if (m_add)
    {
        if (m_replace && (!m_stateStack.empty()))
        {
            m_stateStack.pop();
            m_replace = false;
        }

        m_stateStack.push(std::move(m_newState));
        m_stateStack.top()->init(window);
        m_add = false;
    }
}

const std::unique_ptr<state>& stateSystem::getCurrentState() const noexcept { return this->m_stateStack.top(); }

const std::size_t stateSystem::getCurrentSize() const noexcept { return this->m_stateStack.size(); }