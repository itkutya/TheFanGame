#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <stack>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "SFML/Graphics/RenderWindow.hpp"

#include "Utility.h"

class Application;

class State
{
public:
    State() noexcept = default;
    virtual ~State() noexcept = default;
    virtual void init(sf::RenderWindow& window) = 0;
    virtual void processEvent(sf::Event& event) noexcept = 0;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept = 0;
    virtual void draw(sf::RenderWindow& window) noexcept = 0;

    Application* m_app = nullptr;
};

class GUIState : public State
{
public:
    GUIState() noexcept = default;
    virtual ~GUIState() noexcept = default;
    virtual void init(sf::RenderWindow& window) override {};
    virtual void processEvent(sf::Event& event) noexcept override {};
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override {};
    virtual void draw(sf::RenderWindow& window) noexcept override {};
};

class StateManager : NonCopyable
{
    typedef std::unique_ptr<State> UState;
    typedef std::unique_ptr<GUIState> UGUIState;
public:
    StateManager() noexcept = default;
    ~StateManager() noexcept = default;

    template<typename T> void add(Application* app = nullptr, bool replace = false) noexcept;
    template<typename T> void addGUIState(Application* app = nullptr, bool replace = false) noexcept;

    void popCurrentGUIState() noexcept;
    void popCurrentState() noexcept;

    void processStateChanges(sf::RenderWindow& window) noexcept;

    [[nodiscard]] const std::unique_ptr<State>& getCurrentState() const noexcept;
    [[nodiscard]] const std::vector<UGUIState>& getCurrentGUIStates() noexcept;
    [[nodiscard]] const std::size_t getSize() const noexcept;
private:
    std::stack<std::pair<UState, std::vector<UGUIState>>> m_stack;
    
    UState m_newState;
    bool m_addState = false;
    bool m_replaceState = false;
    bool m_removeState = false;

    UGUIState m_newGUIState;
    bool m_addGUIState = false;
    bool m_replaceGUIState = false;
    bool m_removeGUIState = false;
};

template<typename T>
inline void StateManager::add(Application* app, bool replace) noexcept
{
    this->m_addState = true;
    this->m_replaceState = replace;
    this->m_newState = std::make_unique<T>(app);
}

template<typename T>
inline void StateManager::addGUIState(Application* app, bool replace) noexcept
{
    this->m_addGUIState = true;
    this->m_replaceGUIState = replace;
    this->m_newGUIState = std::make_unique<T>(app);
}

class PopupGUIState : public GUIState
{
public:
    bool m_once = true;
    bool m_open = false;

    virtual void close() noexcept;
};