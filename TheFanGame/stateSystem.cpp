#include "stateSystem.h"

const void stateSystem::popCurrent() noexcept { this->m_remove = true; }

const void stateSystem::processStateChange(sf::RenderWindow& window) noexcept
{
    if (this->m_remove && this->m_stateStack.size())
    {
        this->m_stateStack.pop();
        this->m_remove = false;
    }

    if (this->m_add)
    {
        if (this->m_replace && this->m_stateStack.size())
        {
            this->m_stateStack.pop();
            this->m_replace = false;
        }

        this->m_stateStack.push(std::move(this->m_newState));
        this->m_stateStack.top()->init(window);
        this->m_add = false;
    }
}

const std::unique_ptr<state>& stateSystem::getState() noexcept { return m_stateStack.top(); }

const std::size_t stateSystem::getSize() noexcept { return m_stateStack.size(); }