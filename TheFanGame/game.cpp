#include "game.h"

game::game() noexcept
{
    this->m_texture = nullptr;
    this->m_char = nullptr;
}

game::~game() noexcept {}

const void game::init(window& window) 
{
    this->m_texture = &resourceSystem::c_get<sf::Texture>("WallTexture");
    this->m_char = &resourceSystem::c_get<sf::Texture>("CharacterTexture");
    
    this->m_player = std::make_unique<player>(sf::Vector2f((float)this->m_map.mapSize.x / 2.f, (float)this->m_map.mapSize.y / 2.f), sf::Vector2f(20.f, 20.f), sf::Color::Blue);
    this->m_ray = std::make_unique<ray>(window.getWindow().getSize().x);

    for (std::size_t i = 0; i < 10; ++i)
        this->m_entities.push_back(entity(sf::Vector2f((float)this->m_map.mapSize.x / 2.f, (float)this->m_map.mapSize.y / 2.f), sf::Vector2f(10.f + (std::rand() % 50), 10.f + (std::rand() % 50)), sf::Color::Green));

    this->spriteOrder.resize(this->m_entities.size());
    this->spriteDistance.resize(this->m_entities.size());
    this->zBuffer.resize(window.getWindow().getSize().x);
}

const void game::processEvent(const sf::Event& event) noexcept 
{
    ImGui::SFML::ProcessEvent(event);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        stateSystem::popCurrent();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
    {
        this->m_entities.push_back(entity(sf::Vector2f((float)this->m_map.mapSize.x / 2.f, (float)this->m_map.mapSize.y / 2.f), sf::Vector2f(10.f + (std::rand() % 50), 10.f + (std::rand() % 50)), sf::Color::Green));
        this->spriteOrder.resize(this->m_entities.size());
        this->spriteOrder.shrink_to_fit();
        this->spriteDistance.resize(this->m_entities.size());
        this->spriteDistance.shrink_to_fit();
        for (std::uint32_t i = 0; i < this->m_entities.size(); ++i)
        {
            this->spriteOrder[i] = i;
            this->spriteDistance[i] = std::hypotf(this->m_player->getPosition().x - this->m_entities[i].getPosition().x, this->m_player->getPosition().y - this->m_entities[i].getPosition().y);
        }
        this->sortSprites(this->spriteOrder, this->spriteDistance, this->m_entities.size());
    }

    if (event.type == sf::Event::Resized)
    {
        this->m_ray->r_vertices.resize(static_cast<std::size_t>(event.size.width) + 1);
        this->m_ray->r_walls.resize((static_cast<std::size_t>(event.size.width)) * 2);
        this->m_ray->r_floors.resize((static_cast<std::size_t>(event.size.width)) * 2);
        this->m_ray->r_ceilings.resize((static_cast<std::size_t>(event.size.width)) * 2);

        for (std::size_t i = 0; i < this->m_ray->r_floors.getVertexCount(); ++i)
            this->m_ray->r_floors[i].color = sf::Color(100, 175, 150, 255);

        for (std::size_t i = 0; i < this->m_ray->r_ceilings.getVertexCount(); ++i)
            this->m_ray->r_ceilings[i].color = sf::Color(125, 175, 200, 255);

        this->zBuffer.resize(static_cast<std::size_t>(event.size.width));
        this->zBuffer.shrink_to_fit();
    }
}

const void game::update(window& window, const sf::Time& dt) noexcept
{
    ImGui::SFML::Update(window.getWindow(), dt);
    ImGui::Begin("FPS Counter", 0, ImGuiWindowFlags_NoCollapse);
    ImGui::Text("FPS: %f", 1.f / dt.asSeconds());
    ImGui::Text("Ammo: %i %c %i", this->m_player->ak47.w_currAmmo, '//', this->m_player->ak47.w_maxAmmoCap);
    if (ImGui::BeginListBox("Entities: "))
    {
        for (int i = 0; i < this->m_entities.size(); ++i)
        {
            ImGui::Text("Health: %i %f", i, this->m_entities[i].health);
        }
        ImGui::EndListBox();
    }
    ImGui::End();

    if (window.getWindow().hasFocus() && !sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window.getWindow());
        this->m_player->update(this->m_map, mousePos, window.getWindow().getSize(), this->m_entities, dt);
        sf::Mouse::setPosition(sf::Vector2i(window.getWindow().getSize().x / 2, window.getWindow().getSize().y / 2), window.getWindow());
    }

    this->m_ray->r_vertices[0].position = this->m_player->getPosition();
    for (std::uint32_t i = 0; i < window.getWindow().getSize().x; ++i)
    {
        this->m_ray->castRay(*this->m_player, this->m_map, window.getWindow().getSize(), i);
        this->zBuffer[i] = this->m_ray->perpWallDist * ((this->m_map.mapSize.x + this->m_map.mapSize.y) / 2.f);
    }

    for (std::uint32_t i = 0; i < this->m_entities.size(); ++i)
    {
        this->spriteOrder[i] = i;
        this->spriteDistance[i] = std::hypotf(this->m_player->getPosition().x - this->m_entities[i].getPosition().x, this->m_player->getPosition().y - this->m_entities[i].getPosition().y);
    }
    this->sortSprites(this->spriteOrder, this->spriteDistance, this->m_entities.size());
    
    for (std::vector<entity>::iterator it = this->m_entities.begin(); it != this->m_entities.end(); ++it)
    {
        it->update(*this->m_player, window.getWindow().getSize(), this->zBuffer);
        if (it->health <= 0)
        {
            this->m_entities.erase(it);
            this->spriteOrder.resize(this->m_entities.size());
            this->spriteOrder.shrink_to_fit();
            this->spriteDistance.resize(this->m_entities.size());
            this->spriteDistance.shrink_to_fit();
            for (std::uint32_t i = 0; i < this->m_entities.size(); ++i)
            {
                this->spriteOrder[i] = i;
                this->spriteDistance[i] = std::hypotf(this->m_player->getPosition().x - this->m_entities[i].getPosition().x, this->m_player->getPosition().y - this->m_entities[i].getPosition().y);
            }
            this->sortSprites(this->spriteOrder, this->spriteDistance, this->m_entities.size());
            break;
        }
    }
}

const void game::draw(window& window) noexcept
{
    window.getWindow().setView(window.getView());
    window.getWindow().draw(*this->m_ray, this->m_texture);
    window.getWindow().draw(this->m_map);
    for (std::uint32_t i = 0; i < this->m_entities.size(); ++i)
        window.getWindow().draw(this->m_entities[this->spriteOrder[i]], this->m_char);
    window.getWindow().draw(*this->m_player);

    window.getWindow().setView(window.getWindow().getDefaultView());
    ImGui::SFML::Render(window.getWindow());
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