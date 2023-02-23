#include "StateManager.h"

StateManager& StateManager::getInstance()
{
	static StateManager instance;
	return instance;
}

void StateManager::removeLastGUIState() noexcept
{
    this->m_removeGUI = true;
}

void StateManager::popCurrent() noexcept
{
	this->m_remove = true;
}

void StateManager::processStateChange(sf::RenderWindow& window) noexcept
{
    if (this->m_removeGUI)
    {
        this->m_statestack.top().second.pop_back();
        this->m_removeGUI = false;
    }

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

        this->m_statestack.push(std::make_pair(std::move(this->m_newstate), std::vector<std::unique_ptr<State>>()));
        this->m_statestack.top().first->init(window);
        this->m_add = false;
    }
}

const std::unique_ptr<State>& StateManager::getCurrentState() const noexcept
{
	return this->m_statestack.top().first;
}

std::vector<std::unique_ptr<State>>& StateManager::getCurrentGUIState() noexcept
{
    return this->m_statestack.top().second;
}

const std::size_t StateManager::getSize() const noexcept
{
	return this->m_statestack.size();
}