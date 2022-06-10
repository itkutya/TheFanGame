#include "game.h"

#define texWidth 64
#define texHeight 64

game::game(context* context) noexcept
{
    this->m_context = context;
}

game::~game() noexcept {}

const void game::init(sf::RenderWindow& window) 
{ 
    this->m_Player.setSize(sf::IntRect(-this->m_Map.mapSize.x / 2, -this->m_Map.mapSize.y / 2,
                                        this->m_Map.mapSize.x / 2, this->m_Map.mapSize.y / 2));
    this->m_Player.setPosition(sf::Vector2f(20.f, 20.f));

    this->m_state = &this->m_context->g_resources.getTexture(0);

    this->m_walls.setPrimitiveType(sf::PrimitiveType::Lines);
    this->m_walls.resize((static_cast<std::size_t>(window.getSize().x) + 1) * 2);

    for (std::size_t i = 0; i < 10; i++)
    {
        this->m_entities.push_back(entity(sf::Vector2f((float)this->m_Map.mapSize.x, (float)this->m_Map.mapSize.y), sf::Vector2f((float)(std::rand() % 100), (float)(std::rand() % 100)), sf::Color::Green));
    }
    //this->m_entities.push_back(entity(sf::Vector2f((float)this->m_Map.mapSize.x, (float)this->m_Map.mapSize.y), sf::Vector2f(15.f, 15.f), sf::Color::Green));
    //this->m_entities.push_back(entity(sf::Vector2f((float)this->m_Map.mapSize.x, (float)this->m_Map.mapSize.y), sf::Vector2f(50.f, 15.f), sf::Color::Green));

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
        this->m_walls.resize((static_cast<std::size_t>(event.size.width) + 1) * 2);

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
        float sensitivity = dt.asSeconds() * 30.f;
        float moveSpeed = dt.asSeconds() * 10.f;

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        float diffX = (float)(((window.getSize().x / 2.f) - mousePos.x) / window.getSize().x);
        float oldDirX = this->m_Player.m_direction.x;
        this->m_Player.m_direction.x = this->m_Player.m_direction.x * std::cos(sensitivity * diffX) - this->m_Player.m_direction.y * std::sin(sensitivity * diffX);
        this->m_Player.m_direction.y = oldDirX * std::sin(sensitivity * diffX) + this->m_Player.m_direction.y * std::cos(sensitivity * diffX);
        float oldPlaneX = this->m_Player.m_plane.x;
        this->m_Player.m_plane.x = this->m_Player.m_plane.x * std::cos(sensitivity * diffX) - this->m_Player.m_plane.y * std::sin(sensitivity * diffX);
        this->m_Player.m_plane.y = oldPlaneX * std::sin(sensitivity * diffX) + this->m_Player.m_plane.y * std::cos(sensitivity * diffX);

        float diffY = (float)(((window.getSize().y / 2.f) - mousePos.y) / window.getSize().y);
        if (diffY > 0.f)
            this->m_Player.m_angle += 1.f * sensitivity * diffY;
        else if (diffY < 0.f)
            this->m_Player.m_angle += 1.f * sensitivity * diffY;

        this->m_Player.m_angle = std::clamp(this->m_Player.m_angle, 0.f, 2.f);

        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            this->m_Player.setPosition(sf::Vector2f(this->m_Player.getPosition().x + this->m_Player.m_direction.x * moveSpeed,
                this->m_Player.getPosition().y + this->m_Player.m_direction.y * moveSpeed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            this->m_Player.setPosition(sf::Vector2f(this->m_Player.getPosition().x - this->m_Player.m_direction.x * moveSpeed,
                this->m_Player.getPosition().y - this->m_Player.m_direction.y * moveSpeed));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            this->m_Player.setPosition(sf::Vector2f(this->m_Player.getPosition().x - ((this->m_Player.m_direction.x * cos(PI / 2) - this->m_Player.m_direction.y * sin(PI / 2)) * moveSpeed),
                this->m_Player.getPosition().y - ((this->m_Player.m_direction.x * sin(PI / 2) + this->m_Player.m_direction.y * cos(PI / 2)) * moveSpeed)));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            this->m_Player.setPosition(sf::Vector2f(this->m_Player.getPosition().x + ((this->m_Player.m_direction.x * cos(PI / 2) - this->m_Player.m_direction.y * sin(PI / 2)) * moveSpeed),
                this->m_Player.getPosition().y + ((this->m_Player.m_direction.x * sin(PI / 2) + this->m_Player.m_direction.y * cos(PI / 2)) * moveSpeed)));
        }
    }

    this->m_view.setSize((float)window.getSize().x, (float)window.getSize().y);
    this->m_view.setCenter((float)window.getSize().x / 2.f, (float)window.getSize().y / 2.f);
    this->m_Ray[0].position = this->m_Player.getPosition();
    for (std::uint32_t i = 1; i < window.getSize().x + 1; ++i)
    {
        this->m_Ray.castRay(this->m_Player, this->m_Map, window.getSize().x, window.getSize().y, i);

        sf::Vertex* line = &this->m_walls[static_cast<std::size_t>(i) * 2];
        line[0].position = sf::Vector2f((float)i, (float)this->m_Ray.r_drawPos.x);
        line[1].position = sf::Vector2f((float)i, (float)this->m_Ray.r_drawPos.y);

        sf::Color color;
        color = sf::Color(255 - (int)(this->m_Ray.perpWallDist * 20 > 255 ? 255 : this->m_Ray.perpWallDist * 20),
                          255 - (int)(this->m_Ray.perpWallDist * 20 > 255 ? 255 : this->m_Ray.perpWallDist * 20),
                          255 - (int)(this->m_Ray.perpWallDist * 20 > 255 ? 255 : this->m_Ray.perpWallDist * 20), 255);

        if (this->m_Ray.side)
        {
            color.r = sf::Uint8(color.r / 2.5f);
            color.g = sf::Uint8(color.g / 2.5f);
            color.b = sf::Uint8(color.b / 2.5f);
        }

        line[0].color = color;
        line[1].color = color;

        float wallX;
        if (!this->m_Ray.side) wallX = this->m_Player.getPosition().y / this->m_Map.mapSize.y + this->m_Ray.perpWallDist * this->m_Ray.r_rayDir.y;
        else                   wallX = this->m_Player.getPosition().x / this->m_Map.mapSize.x + this->m_Ray.perpWallDist * this->m_Ray.r_rayDir.x;
        wallX -= floor((wallX));

        int texX = int(wallX * texWidth);
        if (!this->m_Ray.side && this->m_Ray.r_rayDir.x > 0.f) texX = texWidth - texX - 1;
        if (this->m_Ray.side && this->m_Ray.r_rayDir.y < 0.f) texX = texWidth - texX - 1;

        int mapNum = this->m_Map.getMapTile(this->m_Ray.r_mapPos.x, this->m_Ray.r_mapPos.y);
        line[0].texCoords = sf::Vector2f(texX + (texWidth * mapNum + 0.5f), 0.f);
        line[1].texCoords = sf::Vector2f(texX + (texWidth * mapNum + 0.5f), (float)texHeight);

        this->zBuffer[static_cast<std::vector<float, std::allocator<float>>::size_type>(i) - 1] = this->m_Ray.perpWallDist * ((this->m_Map.mapSize.x + this->m_Map.mapSize.y) / 2.f);
    }
    std::vector<int> spriteOrder(this->m_entities.size());
    std::vector<float> spriteDistance(this->m_entities.size());

    for (std::uint32_t i = 0; i < this->m_entities.size(); ++i)
    {
        spriteOrder[i] = i;
        spriteDistance[i] = ((this->m_Player.getPosition().x - this->m_entities[i].getPosition().x) * (this->m_Player.getPosition().x - this->m_entities[i].getPosition().x)
                          + (this->m_Player.getPosition().y - this->m_entities[i].getPosition().y) * (this->m_Player.getPosition().y - this->m_entities[i].getPosition().y));
    }
    this->sortSprites(spriteOrder, spriteDistance, this->m_entities.size());

    for (int a = 0; a < this->m_entities.size(); ++a)
    {
        float spriteX = this->m_entities[spriteOrder[a]].getPosition().x - this->m_Player.getPosition().x;
        float spriteY = this->m_entities[spriteOrder[a]].getPosition().y - this->m_Player.getPosition().y;
        float invDet = 1.f / (this->m_Player.m_plane.x * this->m_Player.m_direction.y - this->m_Player.m_direction.x * this->m_Player.m_plane.y);
        float transformX = invDet * (this->m_Player.m_direction.y * spriteX - this->m_Player.m_direction.x * spriteY);
        float transformY = invDet * (-this->m_Player.m_plane.y * spriteX + this->m_Player.m_plane.x * spriteY);

        int spriteScreenX = (int)((window.getSize().x / 2.f) * (1.f + (transformX / transformY)));

        int vMoveScreen = int(this->m_entities[spriteOrder[a]].m_size.z / transformY);

        int spriteHeight = abs((int)((window.getSize().y / (transformY)) / this->m_entities[spriteOrder[a]].m_size.y));
        int drawStartY = (int)(-spriteHeight / 2.f + window.getSize().y / 2.f * this->m_Player.m_angle + vMoveScreen);
        int drawEndY = (int)(spriteHeight / 2.f + window.getSize().y / 2.f * this->m_Player.m_angle + vMoveScreen);

        int spriteWidth = abs((int)((window.getSize().y / (transformY)) / this->m_entities[spriteOrder[a]].m_size.x));
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

        sf::Vertex* quad = &m_sprites[static_cast<std::size_t>(a) * 4];
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

            quad[0].color = sf::Color(255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5),
                255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5),
                255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5), 255);
            quad[1].color = sf::Color(255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5),
                255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5),
                255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5), 255);
            quad[2].color = sf::Color(255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5),
                255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5),
                255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5), 255);
            quad[3].color = sf::Color(255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5),
                255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5),
                255 - (int)((std::hypot(spriteX, spriteY) * 5) > 255 ? 255 : std::hypot(spriteX, spriteY) * 5), 255);

            if (firstX + 1 < drawEndX && lastX > drawStartX)
            {
                quad[0].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth * (1 - diff)), 0.f);
                quad[1].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth - texWidth * (1 - diff)), 0.f);
                quad[2].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth - texWidth * (1 - diff)), (float)texHeight);
                quad[3].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth * (1 - diff)), (float)texHeight);
            }
            else if (lastX > drawStartX)
            {
                quad[0].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth * (1 - diff)), 0.f);
                quad[1].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth), 0.f);
                quad[2].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth), (float)texHeight);
                quad[3].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth * (1 - diff)), (float)texHeight);
            }
            else if (firstX + 1 < drawEndX)
            {
                quad[0].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture), 0.f);
                quad[1].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth - texWidth * (1 - diff)), 0.f);
                quad[2].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth - texWidth * (1 - diff)), (float)texHeight);
                quad[3].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture), (float)texHeight);
            }
            else
            {
                //Fix maybie???
                quad[0].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture), 0.f);
                quad[1].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth), 0.f);
                quad[2].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture + texWidth), (float)texHeight);
                quad[3].texCoords = sf::Vector2f((float)(texWidth * this->m_entities[spriteOrder[a]].m_texture), (float)texHeight);
            }
        }
    }
}

const void game::draw(sf::RenderWindow& window) noexcept 
{
    window.setView(this->m_view);
    window.draw(this->m_walls, this->m_state);
    window.draw(this->m_sprites, this->m_state);

    window.setView(window.getDefaultView());
    window.draw(this->m_Map);
    window.draw(this->m_Player);
    window.draw(this->m_Ray);
    for (auto& i : this->m_entities)
        window.draw(i);

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