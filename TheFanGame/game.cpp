#include "game.h"

game::game(context* context) noexcept
{
    this->m_context = context;
}

game::~game() noexcept {}

const void game::init(sf::RenderWindow& window) {}

const void game::processEvent(const sf::Event& event) noexcept 
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        this->m_context->g_states.popCurrent();
}

const void game::update(sf::RenderWindow& window, const sf::Time& dt) noexcept 
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        this->miniPlayer.move(0.f, -100.f * dt.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        this->miniPlayer.move(0.f, 100.f * dt.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->miniPlayer.move(-100.f * dt.asSeconds(), 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        this->miniPlayer.move(100.f * dt.asSeconds(), 0.f);

    this->playerRay.castRay(&this->miniPlayer, &this->miniMap);
}

const void game::draw(sf::RenderWindow& window) noexcept 
{
    window.draw(this->miniMap);
    window.draw(this->miniPlayer);
    window.draw(this->playerRay);
}
