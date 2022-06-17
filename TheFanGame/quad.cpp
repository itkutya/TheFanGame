#include "quad.h"

quad::quad(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Color& color) noexcept
{
    this->m_sprites.setPrimitiveType(sf::PrimitiveType::Quads);
    this->m_sprites.resize(4);

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
}

quad::~quad() noexcept {}

const void quad::update(entity& player, const sf::Vector2u& windowSize, const std::vector<float>& zBuffer) noexcept
{
    float spriteX = this->getPosition().x - player.getPosition().x;
    float spriteY = this->getPosition().y - player.getPosition().y;
    float invDet = 1.f / (player.m_plane.x * player.m_direction.y - player.m_direction.x * player.m_plane.y);
    float transformX = invDet * (player.m_direction.y * spriteX - player.m_direction.x * spriteY);
    float transformY = invDet * (-player.m_plane.y * spriteX + player.m_plane.x * spriteY);

    int spriteScreenX = (int)((windowSize.x / 2.f) * (1.f + (transformX / transformY)));

    int vMoveScreen = int(this->m_size.z / transformY);

    int spriteHeight = abs((int)((windowSize.y / (transformY)) / this->m_size.y));
    int drawStartY = (int)(-spriteHeight / 2.f + windowSize.y / 2.f - player.m_angle + vMoveScreen);
    int drawEndY = (int)(spriteHeight / 2.f + windowSize.y / 2.f - player.m_angle + vMoveScreen);

    int spriteWidth = abs((int)((windowSize.y / (transformY)) / this->m_size.x));
    int drawStartX = (int)(-spriteWidth / 2.f + spriteScreenX);
    int drawEndX = (int)(spriteWidth / 2.f + spriteScreenX);

    int firstX = drawStartX;
    int lastX = drawEndX;

    for (int stripe = drawStartX; stripe < drawEndX; ++stripe)
    {
        if (transformY > 0 && stripe > 0 && stripe < (int)(windowSize.x) && transformY < zBuffer[stripe])
        {
            if (firstX < stripe)
                firstX = stripe;
            if (lastX > stripe)
                lastX = stripe;
        }
    }

    this->m_sprites[0].position = sf::Vector2f();
    this->m_sprites[1].position = sf::Vector2f();
    this->m_sprites[2].position = sf::Vector2f();
    this->m_sprites[3].position = sf::Vector2f();

    float diff = (firstX - lastX + 1.f) / (drawEndX - drawStartX);

    if (transformY > 0 && firstX > 0 && lastX < (int)(windowSize.x) && transformY < zBuffer[firstX] && transformY < zBuffer[lastX])
    {
        this->m_sprites[0].position = sf::Vector2f((float)lastX - 1.f, (float)drawStartY);
        this->m_sprites[1].position = sf::Vector2f((float)firstX + 1.f, (float)drawStartY);
        this->m_sprites[2].position = sf::Vector2f((float)firstX + 1.f, (float)drawEndY);
        this->m_sprites[3].position = sf::Vector2f((float)lastX - 1.f, (float)drawEndY);

        this->m_sprites[0].color = sf::Color(
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f),
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f),
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f), 255);
        this->m_sprites[1].color = sf::Color(
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f),
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f),
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f), 255);
        this->m_sprites[2].color = sf::Color(
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f),
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f),
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f), 255);
        this->m_sprites[3].color = sf::Color(
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f),
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f),
            255 - (int)(std::hypotf(spriteX, spriteY) / 24.f > 255 ? 255 : std::hypotf(spriteX, spriteY) / 24.f), 255);

        if (firstX + 1 < drawEndX && lastX > drawStartX)
        {
            this->m_sprites[0].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth * (1 - diff)), 0.f);
            this->m_sprites[1].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth - texWidth * (1 - diff)), 0.f);
            this->m_sprites[2].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth - texWidth * (1 - diff)), (float)texHeight);
            this->m_sprites[3].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth * (1 - diff)), (float)texHeight);
        }
        else if (lastX > drawStartX)
        {
            this->m_sprites[0].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth * (1 - diff)), 0.f);
            this->m_sprites[1].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth), 0.f);
            this->m_sprites[2].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth), (float)texHeight);
            this->m_sprites[3].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth * (1 - diff)), (float)texHeight);
        }
        else if (firstX + 1 < drawEndX)
        {
            this->m_sprites[0].texCoords = sf::Vector2f((float)(texWidth * this->m_texture), 0.f);
            this->m_sprites[1].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth - texWidth * (1 - diff)), 0.f);
            this->m_sprites[2].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth - texWidth * (1 - diff)), (float)texHeight);
            this->m_sprites[3].texCoords = sf::Vector2f((float)(texWidth * this->m_texture), (float)texHeight);
        }
        else
        {
            //Fix maybie???
            this->m_sprites[0].texCoords = sf::Vector2f((float)(texWidth * this->m_texture), 0.f);
            this->m_sprites[1].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth), 0.f);
            this->m_sprites[2].texCoords = sf::Vector2f((float)(texWidth * this->m_texture + texWidth), (float)texHeight);
            this->m_sprites[3].texCoords = sf::Vector2f((float)(texWidth * this->m_texture), (float)texHeight);
        }
    }
}

const void quad::setSize(const sf::IntRect& size) noexcept
{
	this->m_vertices[0].position = sf::Vector2f((float)size.left, (float)size.top);
	this->m_vertices[1].position = sf::Vector2f((float)size.width, (float)size.top);
	this->m_vertices[2].position = sf::Vector2f((float)size.width, (float)size.height);
	this->m_vertices[3].position = sf::Vector2f((float)size.left, (float)size.height);
}
