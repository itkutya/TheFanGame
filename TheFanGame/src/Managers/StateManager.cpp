#include "StateManager.h"

#include "Application.h"

void StateManager::popCurrentGUIState() noexcept
{
    this->m_removeGUIState = true;
}

void StateManager::popCurrentState() noexcept
{
    this->m_removeState = true;
}

void StateManager::processStateChanges(sf::RenderWindow& window) noexcept
{
    if (this->m_removeState && this->m_stack.size())
    {
        this->m_removeState = false;
        this->m_stack.pop();
    }

    if (this->m_addState)
    {
        if (this->m_replaceState && this->m_stack.size())
        {
            this->m_replaceState = false;
            this->m_stack.pop();
        }

        this->m_addState = false;
        const auto& newState = this->m_stack.emplace(std::make_pair(std::move(this->m_newState), std::vector<StateManager::UGUIState>()));
        newState.first->init(window);
    }

    if (this->m_removeGUIState && this->m_stack.top().second.size())
    {
        this->m_removeGUIState = false;
        this->m_stack.top().second.pop_back();
    }

    if (this->m_addGUIState)
    {
        if (this->m_replaceGUIState && this->m_stack.top().second.size())
        {
            this->m_replaceGUIState = false;
            this->m_stack.top().second.pop_back();
        }

        this->m_addGUIState = false;
        const auto& newState = this->m_stack.top().second.emplace_back(std::move(this->m_newGUIState));
        newState->init(window);
    }
}

const std::unique_ptr<State>& StateManager::getCurrentState() const noexcept
{
	return this->m_stack.top().first;
}

const std::vector<StateManager::UGUIState>& StateManager::getCurrentGUIStates() noexcept
{
    return this->m_stack.top().second;
}

const std::size_t StateManager::getSize() const noexcept
{
	return this->m_stack.size();
}

void PopupGUIState::close() noexcept
{
    ImGui::CloseCurrentPopup();
    this->m_app->m_StateManager.popCurrentGUIState();
}