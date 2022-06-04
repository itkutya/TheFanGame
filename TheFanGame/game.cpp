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
    this->miniPlayer.setPosition(sf::Vector2f(20.f, 20.f));

    this->walls.setPrimitiveType(sf::PrimitiveType::Lines);
    this->walls.resize((window.getSize().x + 1) * 2);
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
    
    if (window.hasFocus())
    {
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

        float sensitivity = dt.asSeconds() * 50.f;
        float moveSpeed = dt.asSeconds() * 30.f;

        float diff_X = (float)((mousePos.x - (window.getSize().x / 2)) / window.getSize().x);
        float oldDirX = this->miniPlayer.direction.x;
        this->miniPlayer.direction.x = this->miniPlayer.direction.x * cos(sensitivity * diff_X) - this->miniPlayer.direction.y * sin(sensitivity * diff_X);
        this->miniPlayer.direction.y = oldDirX * sin(sensitivity * diff_X) + this->miniPlayer.direction.y * cos(sensitivity * diff_X);
        float oldPlaneX = this->miniPlayer.plane.x;
        this->miniPlayer.plane.x = this->miniPlayer.plane.x * cos(sensitivity * diff_X) - this->miniPlayer.plane.y * sin(sensitivity * diff_X);
        this->miniPlayer.plane.y = oldPlaneX * sin(sensitivity * diff_X) + this->miniPlayer.plane.y * cos(sensitivity * diff_X);

        float diff_Y = (float)(((window.getSize().y / 2) - mousePos.y) / window.getSize().y);
        if (diff_Y > 0.f)
            this->miniPlayer.angle += 1.f * sensitivity * diff_Y;
        else if (diff_Y < 0.f)
            this->miniPlayer.angle += 1.f * sensitivity * diff_Y;

        this->miniPlayer.angle = std::clamp(this->miniPlayer.angle, 0.f, 2.f);

        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            this->miniPlayer.setPosition(sf::Vector2f(this->miniPlayer.getPosition().x + this->miniPlayer.direction.x * moveSpeed, 
                                                      this->miniPlayer.getPosition().y + this->miniPlayer.direction.y * moveSpeed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            this->miniPlayer.setPosition(sf::Vector2f(this->miniPlayer.getPosition().x - this->miniPlayer.direction.x * moveSpeed,
                                                      this->miniPlayer.getPosition().y - this->miniPlayer.direction.y * moveSpeed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            this->miniPlayer.setPosition(sf::Vector2f(this->miniPlayer.getPosition().x - ((this->miniPlayer.direction.x * cos(PI / 2) - this->miniPlayer.direction.y * sin(PI / 2)) * moveSpeed),
                                                      this->miniPlayer.getPosition().y - ((this->miniPlayer.direction.x * sin(PI / 2) + this->miniPlayer.direction.y * cos(PI / 2)) * moveSpeed)));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            this->miniPlayer.setPosition(sf::Vector2f(this->miniPlayer.getPosition().x + ((this->miniPlayer.direction.x * cos(PI / 2) - this->miniPlayer.direction.y * sin(PI / 2)) * moveSpeed),
                                                      this->miniPlayer.getPosition().y + ((this->miniPlayer.direction.x * sin(PI / 2) + this->miniPlayer.direction.y * cos(PI / 2)) * moveSpeed)));
        }
    }

    this->playerRay[0].position = this->miniPlayer.getPosition();
    for (unsigned int i = 1; i < window.getSize().x + 1; ++i)
    {
        this->playerRay.castRay(&this->miniPlayer, &this->miniMap, window.getSize().x, window.getSize().y, i, this->miniPlayer.direction);

        sf::Vertex* line = &this->walls[i * 2];
        line[0].position = sf::Vector2f((float)i, (float)this->playerRay.getDraw().x);
        line[1].position = sf::Vector2f((float)i, (float)this->playerRay.getDraw().y);

        sf::Color color = sf::Color(255 - (int)(this->playerRay.getDistance() * 10 > 255 ? 255 : this->playerRay.getDistance() * 10),
                                    255 - (int)(this->playerRay.getDistance() * 10 > 255 ? 255 : this->playerRay.getDistance() * 10),
                                    255 - (int)(this->playerRay.getDistance() * 10 > 255 ? 255 : this->playerRay.getDistance() * 10), 255);

        line[0].color = color;
        line[1].color = color;
    }
}

const void game::draw(sf::RenderWindow& window) noexcept 
{
    window.draw(this->walls);
    window.draw(this->miniMap);
    window.draw(this->miniPlayer);
    window.draw(this->playerRay);

    ImGui::SFML::Render(window);
}
