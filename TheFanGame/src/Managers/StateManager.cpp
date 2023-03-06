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
    if (this->m_remove && this->m_statestack.size())
    {
        this->m_remove = false;
        this->m_statestack.pop();
    }

    if (this->m_add)
    {
        if (this->m_replace && this->m_statestack.size())
        {
            this->m_replace = false;
            this->m_statestack.pop();
        }

        this->m_add = false;
        auto& newState = this->m_statestack.emplace(std::make_pair(std::move(this->m_newstate), std::vector<std::unique_ptr<State>>()));
        newState.first->init(window);
    }

    if (this->m_statestack.top().second.size() && this->m_removeGUI)
    {
        this->m_removeGUI = false;
        this->m_statestack.top().second.pop_back();
    }

    if (this->m_addGUI)
    {
        if (this->m_statestack.top().second.size() && this->m_replaceGUI)
        {
            this->m_replaceGUI = false;
            this->m_statestack.top().second.pop_back();
        }

        this->m_addGUI = false;
        auto& newState = this->m_statestack.top().second.emplace_back(std::move(this->m_newGUIstate));
        newState->init(window);
    }
}

const std::unique_ptr<State>& StateManager::getCurrentState() const noexcept
{
	return this->m_statestack.top().first;
}

const std::vector<std::unique_ptr<State>>& StateManager::getCurrentGUIState() noexcept
{
    return this->m_statestack.top().second;
}

const std::size_t StateManager::getSize() const noexcept
{
	return this->m_statestack.size();
}