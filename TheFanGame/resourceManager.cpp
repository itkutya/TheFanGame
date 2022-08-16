#include "resourceManager.h"

const void resourceManager::addTexture(const std::uint8_t& id, const std::string& filePath, const bool& wantRepeated)
{
    this->m_textures[id] = std::make_unique<sf::Texture>();

    if (!this->m_textures[id]->loadFromFile(filePath))
        throw "Cannot load texture...\n";
        
    this->m_textures[id]->setRepeated(wantRepeated);
}

const void resourceManager::addFont(const std::uint8_t& id, const std::string& filePath)
{
    this->m_fonts[id] = std::make_unique<sf::Font>();

    if (!this->m_fonts[id]->loadFromFile(filePath))
        throw "Cannot load font...\n";
}

const void resourceManager::addSoundBuffer(const std::uint8_t& id, const std::string& filePath)
{
    this->m_soundBuffers[id] = std::make_unique<sf::SoundBuffer>();

    if (!this->m_soundBuffers[id]->loadFromFile(filePath))
        throw "Cannot load sound...\n";
}

const sf::Texture& resourceManager::getTexture(const std::uint8_t& id) const { return *(this->m_textures.at(id).get()); }

const sf::Font& resourceManager::getFont(const std::uint8_t& id) const { return *(this->m_fonts.at(id).get()); }

const sf::SoundBuffer& resourceManager::getSoundBuffer(const std::uint8_t& id) const { return *(this->m_soundBuffers.at(id).get()); }