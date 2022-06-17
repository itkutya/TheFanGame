#include "game.h"

game::game(context* context) noexcept
{
    this->m_context = context;
    this->m_texture = nullptr;
}

game::~game() noexcept {}

const void game::init(sf::RenderWindow& window) 
{ 
    this->m_player.setSize(sf::IntRect(-this->m_map.mapSize.x / 2, -this->m_map.mapSize.y / 2,
                                        this->m_map.mapSize.x / 2, this->m_map.mapSize.y / 2));
    this->m_player.setPosition(sf::Vector2f(20.f, 20.f));

    this->m_texture = &this->m_context->g_resources.getTexture(0);

    for (std::size_t i = 0; i < 10; ++i)
        this->m_entities.push_back(entity(sf::Vector2f((float)this->m_map.mapSize.x, (float)this->m_map.mapSize.y), sf::Vector2f((float)(std::rand() % 100), (float)(std::rand() % 100)), sf::Color::Green));

    this->m_sprites.setPrimitiveType(sf::PrimitiveType::Quads);
    this->m_sprites.resize(this->m_entities.size() * 4);

    this->zBuffer.resize(window.getSize().x);
}

const void game::processEvent(const sf::Event& event) noexcept 
{
    ImGui::SFML::ProcessEvent(event);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        this->m_context->g_states.popCurrent();

    if (event.type == sf::Event::Resized)
    {
        this->zBuffer.resize(static_cast<std::size_t>(event.size.width));
        this->zBuffer.shrink_to_fit();
    }
}

const void game::update(sf::RenderWindow& window, const sf::Time& dt) noexcept
{
    if ((this->m_sprites.getVertexCount() / 4) != this->m_entities.size())
        this->m_sprites.resize(this->m_entities.size() * 4);

    ImGui::SFML::Update(window, dt);
    ImGui::Begin("FPS Counter", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::Text("FPS: %f", 1.f / dt.asSeconds());
    ImGui::End();

    if (window.hasFocus() && !sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        this->m_player.update(mousePos, window.getSize(), dt);
        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
    }

    this->m_view.setSize((float)window.getSize().x, (float)window.getSize().y);
    this->m_view.setCenter((float)window.getSize().x / 2.f, (float)window.getSize().y / 2.f);
    this->m_ray[0].position = this->m_player.getPosition();
    for (std::uint32_t i = 0; i < window.getSize().x; ++i)
    {
        this->m_ray.castRay(this->m_player, this->m_map, window.getSize().x, window.getSize().y, i);
        this->zBuffer[i] = this->m_ray.perpWallDist * ((this->m_map.mapSize.x + this->m_map.mapSize.y) / 2.f);
    }

    std::vector<int> spriteOrder(this->m_entities.size());
    std::vector<float> spriteDistance(this->m_entities.size());

    for (std::uint32_t i = 0; i < this->m_entities.size(); ++i)
    {
        spriteOrder[i] = i;
        spriteDistance[i] = ((this->m_player.getPosition().x - this->m_entities[i].getPosition().x) * (this->m_player.getPosition().x - this->m_entities[i].getPosition().x)
                          + (this->m_player.getPosition().y - this->m_entities[i].getPosition().y) * (this->m_player.getPosition().y - this->m_entities[i].getPosition().y));
    }
    this->sortSprites(spriteOrder, spriteDistance, this->m_entities.size());

    for (std::uint32_t i = 0; i < this->m_entities.size(); ++i)
    {
        float spriteX = this->m_entities[spriteOrder[i]].getPosition().x - this->m_player.getPosition().x;
        float spriteY = this->m_entities[spriteOrder[i]].getPosition().y - this->m_player.getPosition().y;
        float invDet = 1.f / (this->m_player.m_plane.x * this->m_player.m_direction.y - this->m_player.m_direction.x * this->m_player.m_plane.y);
        float transformX = invDet * (this->m_player.m_direction.y * spriteX - this->m_player.m_direction.x * spriteY);
        float transformY = invDet * (-this->m_player.m_plane.y * spriteX + this->m_player.m_plane.x * spriteY);

        int spriteScreenX = (int)((window.getSize().x / 2.f) * (1.f + (transformX / transformY)));

        int vMoveScreen = int(this->m_entities[spriteOrder[i]].m_size.z / transformY);

        int spriteHeight = abs((int)((window.getSize().y / (transformY)) / this->m_entities[spriteOrder[i]].m_size.y));
        int drawStartY = (int)(-spriteHeight / 2.f + window.getSize().y / 2.f * this->m_player.m_angle + vMoveScreen);
        int drawEndY = (int)(spriteHeight / 2.f + window.getSize().y / 2.f * this->m_player.m_angle + vMoveScreen);

        int spriteWidth = abs((int)((window.getSize().y / (transformY)) / this->m_entities[spriteOrder[i]].m_size.x));
        int drawStartX = (int)(-spriteWidth / 2.f + spriteScreenX);
        int drawEndX = (int)(spriteWidth / 2.f + spriteScreenX);

        int firstX = drawStartX;
        int lastX = drawEndX;

        for (int stripe = drawStartX; stripe < drawEndX; ++stripe)
        {
            if (transformY > 0 && stripe > 0 && stripe < (int)(window.getSize().x) && transformY < zBuffer[stripe])
            {
                if (firstX < stripe)
                    firstX = stripe;
                if (lastX > stripe)
                    lastX = stripe;
            }
        }

        sf::Vertex* quad = &m_sprites[static_cast<std::size_t>(i) * 4];
        quad[0].position = sf::Vector2f();
        quad[1].position = sf::Vector2f();
        quad[2].position = sf::Vector2f();
        quad[3].position = sf::Vector2f();

        float diff = (firstX - lastX + 1.f) / (drawEndX - drawStartX);

        if (transformY > 0 && firstX > 0 && lastX < (int)(window.getSize().x) && transformY < zBuffer[firstX] && transformY < zBuffer[lastX])
        {
            quad[0].position = sf::Vector2f((float)lastX - 1.f, (float)drawStartY);
            quad[1].position = sf::Vector2f((float)firstX + 1.f, (float)drawStartY);
            quad[2].position = sf::Vector2f((float)firstX + 1.f, (float)drawEndY);
            quad[3].position = sf::Vector2f((float)lastX - 1.f, (float)drawEndY);

            quad[0].color = sf::Color(
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f),
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f),
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f), 255);
            quad[1].color = sf::Color(
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f),
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f),
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f), 255);
            quad[2].color = sf::Color(
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f),
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f),
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f), 255);
            quad[3].color = sf::Color(
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f),
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f),
                255 - (int)(spriteDistance[i] / 24.f > 255 ? 255 : spriteDistance[i] / 24.f), 255);
            
            if (firstX + 1 < drawEndX && lastX > drawStartX)
            {
                quad[0].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth * (1 - diff)), 0.f);
                quad[1].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth - texWidth * (1 - diff)), 0.f);
                quad[2].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth - texWidth * (1 - diff)), (float)texHeight);
                quad[3].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth * (1 - diff)), (float)texHeight);
            }
            else if (lastX > drawStartX)
            {
                quad[0].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth * (1 - diff)), 0.f);
                quad[1].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth), 0.f);
                quad[2].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth), (float)texHeight);
                quad[3].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth * (1 - diff)), (float)texHeight);
            }
            else if (firstX + 1 < drawEndX)
            {
                quad[0].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture), 0.f);
                quad[1].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth - texWidth * (1 - diff)), 0.f);
                quad[2].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth - texWidth * (1 - diff)), (float)texHeight);
                quad[3].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture), (float)texHeight);
            }
            else
            {
                //Fix maybie???
                quad[0].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture), 0.f);
                quad[1].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth), 0.f);
                quad[2].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture + texWidth), (float)texHeight);
                quad[3].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[i]].m_texture), (float)texHeight);
            }
        }
    }
}

const void game::draw(sf::RenderWindow& window) noexcept
{
    window.setView(this->m_view);
    window.draw(this->m_ray, this->m_texture);
    window.draw(this->m_sprites, this->m_texture);
    window.draw(this->m_map);
    window.draw(this->m_player);
    for (auto& i : this->m_entities)
        window.draw(i);

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