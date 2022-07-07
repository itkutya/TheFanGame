#include "player.h"

player::player(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Color& color) noexcept
{
	this->m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
	this->m_vertices.resize(4);

	this->m_vertices[0].position = sf::Vector2f(-size.x, -size.y);
	this->m_vertices[1].position = sf::Vector2f(size.x, -size.y);
	this->m_vertices[2].position = sf::Vector2f(size.x, size.y);
	this->m_vertices[3].position = sf::Vector2f(-size.x, size.y);

	this->setPosition(pos);

	this->m_vertices[0].color = color;
	this->m_vertices[1].color = color;
	this->m_vertices[2].color = color;
	this->m_vertices[3].color = color;

	this->m_direction = { -1.f, 0.f };
	this->m_plane = { 0.f, 0.69f };
	this->m_angle = 0.f;
    this->m_texture = 4;
    this->m_size = { 0.5f, 0.5f, 0.f };

    this->movementSpeed = 10.f;
    this->mouseSensivity = 30.f;
}

player::~player() noexcept {}

const void player::update(world& map, const sf::Vector2i& mousePos, const sf::Vector2u& windowSize, std::vector<entity>& entities, const sf::Time& dt) noexcept
{
    float sensitivity = dt.asSeconds() * this->mouseSensivity;
    float moveSpeed = dt.asSeconds() * this->movementSpeed;

    float diffX = (float)(((windowSize.x / 2.f) - mousePos.x) / windowSize.x);
    if (diffX != 0.f)
    {
        float oldDirX = this->m_direction.x;
        this->m_direction.x = this->m_direction.x * std::cos(sensitivity * diffX) - this->m_direction.y * std::sin(sensitivity * diffX);
        this->m_direction.y = oldDirX * std::sin(sensitivity * diffX) + this->m_direction.y * std::cos(sensitivity * diffX);
        float oldPlaneX = this->m_plane.x;
        this->m_plane.x = this->m_plane.x * std::cos(sensitivity * diffX) - this->m_plane.y * std::sin(sensitivity * diffX);
        this->m_plane.y = oldPlaneX * std::sin(sensitivity * diffX) + this->m_plane.y * std::cos(sensitivity * diffX);
    }

    float diffY = (float)(((windowSize.y / 2.f) - mousePos.y) / windowSize.y);
    if (diffY != 0.f)
        this->m_angle -= 300.f * sensitivity * diffY;

    this->m_angle = std::clamp(this->m_angle, -400.f, 400.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        if (map.getMapTile(this->getPosition().x / map.mapSize.x + this->m_direction.x * moveSpeed, this->getPosition().y / map.mapSize.y + this->m_direction.y * moveSpeed) == 0)
            this->move(sf::Vector2f(this->m_direction.x * moveSpeed, this->m_direction.y * moveSpeed));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        if (map.getMapTile(this->getPosition().x / map.mapSize.x + -this->m_direction.x * moveSpeed, this->getPosition().y / map.mapSize.y + -this->m_direction.y * moveSpeed) == 0)
            this->move(sf::Vector2f(-this->m_direction.x * moveSpeed, -this->m_direction.y * moveSpeed));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        if (map.getMapTile(this->getPosition().x / map.mapSize.y + this->m_direction.x * moveSpeed, this->getPosition().y / map.mapSize.y + -this->m_direction.x * moveSpeed) == 0)
            this->move(sf::Vector2f(this->m_direction.y * moveSpeed, -this->m_direction.x * moveSpeed));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        if (map.getMapTile(this->getPosition().x / map.mapSize.x + -this->m_direction.y * moveSpeed, this->getPosition().y / map.mapSize.y + this->m_direction.x * moveSpeed) == 0)
            this->move(sf::Vector2f(-this->m_direction.y * moveSpeed, this->m_direction.x * moveSpeed));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        this->ak47.shoot(*this, map, entities, windowSize);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        this->ak47.reload();
    this->ak47.update(*this, windowSize);
}

const void player::setSize(const sf::IntRect& size) noexcept
{
    this->m_vertices[0].position = sf::Vector2f((float)size.left, (float)size.top);
    this->m_vertices[1].position = sf::Vector2f((float)size.width, (float)size.top);
    this->m_vertices[2].position = sf::Vector2f((float)size.width, (float)size.height);
    this->m_vertices[3].position = sf::Vector2f((float)size.left, (float)size.height);
}

void player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(this->m_sprites, states.texture);
    target.draw(this->m_vertices, states.transform);
    target.draw(this->ak47);
}
