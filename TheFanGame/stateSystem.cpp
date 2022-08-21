#include "stateSystem.h"

std::stack<std::unique_ptr<state>> stateSystem::m_stateStack;
std::unique_ptr<state> stateSystem::m_newState;

bool stateSystem::m_add = false;
bool stateSystem::m_replace = false;
bool stateSystem::m_remove = false;

const void stateSystem::popCurrent() noexcept { m_remove = true; }

const void stateSystem::processStateChange(sf::RenderWindow& window) noexcept
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

const std::unique_ptr<state>& stateSystem::getState() noexcept { return m_stateStack.top(); }

const std::size_t stateSystem::getSize() noexcept { return m_stateStack.size(); }

const void stateSystem::clear()
{
    std::size_t maxSize = getSize();
    for (std::size_t i = 0; i < maxSize; ++i)
        if ((!m_stateStack.empty()))
            m_stateStack.pop();
}