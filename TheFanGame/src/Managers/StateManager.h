#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <stack>
#include <type_traits>

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

template<class T>
concept IState = requires(T a)
{
    std::is_base_of<State, T>{};
};

template<class T>
concept IGUIState = requires(T a)
{
    std::is_base_of<GUIState, T>{};
};

class StateManager : public Singleton<StateManager>
{
    friend class Singleton<StateManager>;

    typedef std::unique_ptr<State> UState;
    typedef std::unique_ptr<GUIState> UGUIState;
protected:
    StateManager() noexcept = default;
    ~StateManager() noexcept = default;
public:
    template<IState T>    void addState(Application* app = nullptr, bool replace = false) noexcept;
    template<IGUIState T> void addGUIState(Application* app = nullptr, bool replace = false) noexcept;

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

template<IState T>
inline void StateManager::addState(Application* app, bool replace) noexcept
{
    this->m_addState = true;
    this->m_replaceState = replace;
    this->m_newState = std::make_unique<T>(app);
}

template<IGUIState T>
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
    const char* m_name = "###";

    virtual inline void init(sf::RenderWindow& window) final
    {
        this->m_open = true;
    }

    virtual inline void draw(sf::RenderWindow& window) noexcept final
    {
        if (this->m_once)
        {
            ImGui::OpenPopup(this->m_name);
            this->m_once = false;
        }
    }

    virtual inline void close() noexcept
    {
        ImGui::CloseCurrentPopup();
        StateManager::getInstance().popCurrentGUIState();
    }
};