#include "game.h"

game::game(context* context) noexcept
{
    this->m_context = context;
}

game::~game() noexcept {}

const void game::init(sf::RenderWindow& window) 
{ 
    this->miniPlayer.setSize(sf::IntRect(-this->miniMap.mapSize.x / 2, -this->miniMap.mapSize.y / 2,
                                          this->miniMap.mapSize.x / 2, this->miniMap.mapSize.y / 2));
}

const void game::processEvent(const sf::Event& event) noexcept 
{
    ImGui::SFML::ProcessEvent(event);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        this->m_context->g_states.popCurrent();
}

const void game::update(sf::RenderWindow& window, const sf::Time& dt) noexcept 
{
    ImGui::SFML::Update(window, dt);

    ImGui::Begin("FPS Counter", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::Text("FPS: %f", 1.f / dt.asSeconds());
    ImGui::End();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        this->miniPlayer.move(0.f, -100.f * dt.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        this->miniPlayer.move(0.f, 100.f * dt.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->miniPlayer.move(-100.f * dt.asSeconds(), 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        this->miniPlayer.move(100.f * dt.asSeconds(), 0.f);

    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    sf::Vector2f mouseDir = (mousePos - this->miniPlayer.getPosition()) / std::hypotf(mousePos.x - this->miniPlayer.getPosition().x, 
                                                                                      mousePos.y - this->miniPlayer.getPosition().y);

    this->playerRay[0].position = this->miniPlayer.getPosition();
    for (unsigned int i = 1; i < window.getSize().x + 1; ++i)
        this->playerRay.castRay(&this->miniPlayer, &this->miniMap, window.getSize().x, window.getSize().y, i, mouseDir);
}

const void game::draw(sf::RenderWindow& window) noexcept 
{
    window.draw(this->miniMap);
    window.draw(this->miniPlayer);
    window.draw(this->playerRay);

    ImGui::SFML::Render(window);
}
