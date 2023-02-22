#include "StateManager.h"

StateManager& StateManager::getInstance()
{
	static StateManager instance;
	return instance;
}

void StateManager::popCurrent() noexcept
{
	this->m_remove = true;
}

void StateManager::processStateChange(sf::RenderWindow& window) noexcept
{
    if (this->m_remove && this->m_statestack.size())
    {
        this->m_statestack.pop();
        this->m_remove = false;
    }

    if (this->m_add)
    {
        if (this->m_replace && this->m_statestack.size())
        {
            this->m_statestack.pop();
            this->m_replace = false;
        }

        this->m_statestack.push(std::move(this->m_newstate));
        this->m_statestack.top()->init(window);
        this->m_add = false;
    }
}

const std::unique_ptr<State>& StateManager::getCurrentState() const noexcept
{
	return this->m_statestack.top();
}

const std::size_t StateManager::getSize() const noexcept
{
	return this->m_statestack.size();
}