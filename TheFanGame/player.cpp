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
}

player::~player() noexcept {}

const void player::update(const sf::Vector2i& mousePos, const sf::Vector2u& windowSize, const sf::Time& dt) noexcept
{
    float sensitivity = dt.asSeconds() * this->mouseSensivity;
    float moveSpeed = dt.asSeconds() * this->movementSpeed;

    float diffX = (float)(((windowSize.x / 2.f) - mousePos.x) / windowSize.x);
    float oldDirX = this->m_direction.x;
    this->m_direction.x = this->m_direction.x * std::cos(sensitivity * diffX) - this->m_direction.y * std::sin(sensitivity * diffX);
    this->m_direction.y = oldDirX * std::sin(sensitivity * diffX) + this->m_direction.y * std::cos(sensitivity * diffX);
    float oldPlaneX = this->m_plane.x;
    this->m_plane.x = this->m_plane.x * std::cos(sensitivity * diffX) - this->m_plane.y * std::sin(sensitivity * diffX);
    this->m_plane.y = oldPlaneX * std::sin(sensitivity * diffX) + this->m_plane.y * std::cos(sensitivity * diffX);

    float diffY = (float)(((windowSize.y / 2.f) - mousePos.y) / windowSize.y);
    if (diffY != 0.f)
        this->m_angle += 1.f * sensitivity * diffY;

    this->m_angle = std::clamp(this->m_angle, 0.f, 2.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        this->setPosition(sf::Vector2f(
            this->getPosition().x + this->m_direction.x * moveSpeed,
            this->getPosition().y + this->m_direction.y * moveSpeed));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        this->setPosition(sf::Vector2f(
            this->getPosition().x - this->m_direction.x * moveSpeed,
            this->getPosition().y - this->m_direction.y * moveSpeed));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        this->setPosition(sf::Vector2f(
            this->getPosition().x - (-this->m_direction.y * moveSpeed),
            this->getPosition().y - (this->m_direction.x * moveSpeed)));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        this->setPosition(sf::Vector2f(
            this->getPosition().x + (-this->m_direction.y * moveSpeed),
            this->getPosition().y + (this->m_direction.x * moveSpeed)));
    }
}
