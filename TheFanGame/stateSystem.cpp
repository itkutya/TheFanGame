#include "stateSystem.h"

const void stateSystem::popCurrent() noexcept { m_remove = true; }

const void stateSystem::processStateChange(sf::RenderWindow& window) noexcept
{
    if (m_remove && m_stateStack.size())
    {
        m_stateStack.pop();
        m_remove = false;
    }

    if (m_add)
    {
        if (m_replace && m_stateStack.size())
        {
            m_stateStack.pop();
            m_replace = false;
        }

        m_stateStack.push(std::move(m_newState));
        m_stateStack.top()->init(window);
        m_add = false;
    }
}

const std::unique_ptr<state>& stateSystem::getState() noexcept { return m_stateStack.top(); }

const std::size_t stateSystem::getSize() noexcept { return m_stateStack.size(); }