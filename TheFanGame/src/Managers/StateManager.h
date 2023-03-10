#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <stack>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "SFML/Graphics.hpp"

class State
{
public:
    explicit State() noexcept = default;
    virtual ~State() noexcept = default;
    virtual void init(sf::RenderWindow& window) {};
    virtual void processEvent(sf::Event& event) noexcept {};
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept {};
    virtual void draw(sf::RenderWindow& window) noexcept {};
};

class StateManager
{
public:
    StateManager(StateManager const&) = delete;
    void operator=(StateManager const&) = delete;
    virtual ~StateManager() noexcept = default;

    [[nodiscard]] static StateManager& getInstance();

    template<typename T> void add(bool replace = false) noexcept;
    template<typename T> void addGUIState(bool replace = false) noexcept;
    void removeLastGUIState() noexcept;

    void popCurrent() noexcept;
    void processStateChange(sf::RenderWindow& window) noexcept;

    [[nodiscard]] const std::unique_ptr<State>& getCurrentState() const noexcept;
    [[nodiscard]] const std::vector<std::unique_ptr<State>>& getCurrentGUIState() noexcept;
    [[nodiscard]] const std::size_t getSize() const noexcept;
private:
    explicit StateManager() noexcept = default;

    std::stack<std::pair<std::unique_ptr<State>, std::vector<std::unique_ptr<State>>>> m_statestack;
    std::unique_ptr<State> m_newstate;

    bool m_add = false;
    bool m_replace = false;
    bool m_remove = false;

    std::unique_ptr<State> m_newGUIstate;
    bool m_addGUI = false;
    bool m_replaceGUI = false;
    bool m_removeGUI = false;
};

template<typename T>
inline void StateManager::add(bool replace) noexcept
{
    this->m_add = true;
    this->m_newstate = std::move(std::make_unique<T>());
    this->m_replace = replace;
}

template<typename T>
inline void StateManager::addGUIState(bool replace) noexcept
{
    this->m_addGUI = true;
    this->m_newGUIstate = std::move(std::make_unique<T>());
    this->m_replaceGUI = replace;
}

class PopUpState
{
public:
    bool m_once = true;
    bool m_open = false;

    virtual inline void close() noexcept
    {
        ImGui::CloseCurrentPopup();
        this->s_StateManager->removeLastGUIState();
    };
protected:
    StateManager* s_StateManager = &StateManager::getInstance();
};