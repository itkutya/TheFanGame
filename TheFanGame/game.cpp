#include "game.h"

game::game() noexcept
{ 
    m_resources = std::make_shared<resourceManager>();
}

game::~game() noexcept { m_resources.reset(); }

const void game::init(sf::RenderWindow& window)
{
    //m_resources->addTexture(0, "res/wolftextures.png");
}

const void game::processEvent(const sf::Event& event) noexcept 
{
    //TODO:...
}

const void game::update(sf::RenderWindow& window, const sf::Time& dt) noexcept {}

const void game::draw(sf::RenderWindow& window) noexcept {}
