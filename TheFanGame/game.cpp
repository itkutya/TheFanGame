#include "game.h"

game::game(context* context) noexcept
{
    this->m_context = context;
    this->m_texture = nullptr;
}

game::~game() noexcept {}

const void game::init(sf::RenderWindow& window) 
{
    this->m_texture = &this->m_context->g_resources.getTexture(0);
    
    this->m_player = std::make_unique<player>(sf::Vector2f((float)this->m_map.mapSize.x / 2.f, (float)this->m_map.mapSize.y / 2.f), sf::Vector2f(20.f, 20.f), sf::Color::Blue);
    this->m_ray = std::make_unique<ray>(window.getSize().x);

    for (std::size_t i = 0; i < 20; ++i)
        this->m_entities.push_back(std::make_unique<quad>(sf::Vector2f((float)this->m_map.mapSize.x / 2.f, (float)this->m_map.mapSize.y / 2.f), sf::Vector2f(10.f + (std::rand() % 50), 10.f + (std::rand() % 50)), sf::Color::Green));

    this->spriteOrder.resize(this->m_entities.size());
    this->spriteDistance.resize(this->m_entities.size());

    this->zBuffer.resize(window.getSize().x);

}

const void game::processEvent(const sf::Event& event) noexcept 
{
    ImGui::SFML::ProcessEvent(event);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        this->m_context->g_states.popCurrent();

    if (event.type == sf::Event::Resized)
    {
        this->m_ray->r_vertices.resize(static_cast<std::size_t>(event.size.width) + 1);
        this->m_ray->r_walls.resize((static_cast<std::size_t>(event.size.width)) * 2);
        this->zBuffer.resize(static_cast<std::size_t>(event.size.width));
        this->zBuffer.shrink_to_fit();
    }
}

const void game::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
    ImGui::SFML::Update(window, dt);
    ImGui::Begin("FPS Counter", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::Text("FPS: %f", 1.f / dt.asSeconds());
    ImGui::End();

    if (window.hasFocus() && !sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        this->m_player->update(mousePos, window.getSize(), dt);
        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
    }

    this->m_view.setSize((float)window.getSize().x, (float)window.getSize().y);
    this->m_view.setCenter((float)window.getSize().x / 2.f, (float)window.getSize().y / 2.f);
    this->m_ray->r_vertices[0].position = this->m_player->getPosition();
    for (std::uint32_t i = 0; i < window.getSize().x; ++i)
    {
        this->m_ray->castRay(*this->m_player, this->m_map, window.getSize().x, window.getSize().y, i);
        this->zBuffer[i] = this->m_ray->perpWallDist * ((this->m_map.mapSize.x + this->m_map.mapSize.y) / 2.f);
    }

    for (std::uint32_t i = 0; i < this->m_entities.size(); ++i)
    {
        this->spriteOrder[i] = i;
        this->spriteDistance[i] = std::hypotf(this->m_player->getPosition().x - this->m_entities[i]->getPosition().x, this->m_player->getPosition().y - this->m_entities[i]->getPosition().y);
    }
    this->sortSprites(this->spriteOrder, this->spriteDistance, this->m_entities.size());
    
    for (std::vector<std::unique_ptr<entity>>::iterator it = this->m_entities.begin(); it != this->m_entities.end(); ++it)
        it->get()->update(*this->m_player, window.getSize(), this->zBuffer);
}

const void game::draw(sf::RenderWindow& window) noexcept
{
    window.setView(this->m_view);
    window.draw(*this->m_ray, this->m_texture);
    window.draw(this->m_map);
    window.draw(*this->m_player);
    for (std::uint32_t i = 0; i < this->m_entities.size(); ++i)
        window.draw(*this->m_entities[this->spriteOrder[i]], this->m_texture);

    window.setView(window.getDefaultView());
    ImGui::SFML::Render(window);
}

const void game::sortSprites(std::vector<int>& order, std::vector<float>& dist, const std::size_t& amount) noexcept
{
    std::vector<std::pair<float, int>> sprites(amount);
    for (int i = 0; i < amount; ++i) {
        sprites[i].first = dist[i];
        sprites[i].second = order[i];
    }

    std::sort(sprites.begin(), sprites.end());

    for (int i = 0; i < amount; ++i) {
        dist[i] = sprites[amount - i - 1].first;
        order[i] = sprites[amount - i - 1].second;
    }
}

/*
* "Special effect"...
sf::CircleShape light(300.f);
light.setOrigin(sf::Vector2f(light.getGlobalBounds().width / 2.f, light.getGlobalBounds().height / 2.f));
light.setPosition(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));
ImGui::Begin("Alpha");
ImGui::Text("Alpha: %f", alpha);
ImGui::SliderFloat("Aplha ", &alpha, 0.f, 255.f);
ImGui::ColorEdit3("Color: ", color);
ImGui::End();
light.setFillColor(sf::Color(color[0] * 255, color[1] * 255, color[2] * 255, alpha));
window.draw(light, sf::BlendMultiply);
*/