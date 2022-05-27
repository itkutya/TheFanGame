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

const void game::update(sf::RenderWindow& window, const sf::Time& dt) noexcept {}

const void game::draw(sf::RenderWindow& window) noexcept {}
