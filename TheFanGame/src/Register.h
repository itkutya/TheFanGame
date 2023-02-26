#pragma once

#include "ResourceManager.h"
#include "StateManager.h"
#include "Account.h"

class Register : public State
{
public:
    explicit Register() noexcept = default;
    virtual ~Register() noexcept = default;

    virtual void init(sf::RenderWindow& window) override;
    virtual void processEvent(const sf::Event& event) noexcept override;
    virtual void update(sf::RenderWindow& window, const sf::Time& dt) noexcept override;
    virtual void draw(sf::RenderWindow& window) noexcept override;

    bool RegisterAccount() noexcept;
private:
    Account* s_Account = &Account::getInstance();
    StateManager* s_StateManager = &StateManager::getInstance();

    bool m_once = true;
    bool m_open = false;

    void close() noexcept;
};