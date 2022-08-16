#include "resourceManager.h"

std::unordered_map<std::string, sf::Texture> resourceManager::m_textures;
std::unordered_map<std::string, sf::Font> resourceManager::m_fonts;
std::unordered_map<std::string, sf::SoundBuffer> resourceManager::m_soundBuffers;

const void resourceManager::addTexture(const std::string& id, const std::string& filePath, const bool& wantRepeated)
{
    m_textures[id] = sf::Texture();

    if (!m_textures[id].loadFromFile(filePath))
        throw "Cannot load texture...\n";
        
    m_textures[id].setRepeated(wantRepeated);
}

const void resourceManager::addFont(const std::string& id, const std::string& filePath)
{
    m_fonts[id] = sf::Font();

    if (!m_fonts[id].loadFromFile(filePath))
        throw "Cannot load font...\n";
}

const void resourceManager::addSoundBuffer(const std::string& id, const std::string& filePath)
{
    m_soundBuffers[id] = sf::SoundBuffer();

    if (!m_soundBuffers[id].loadFromFile(filePath))
        throw "Cannot load sound...\n";
}

const sf::Texture& resourceManager::getTexture(const std::string& id) { return m_textures.at(id); }

const sf::Font& resourceManager::getFont(const std::string& id) { return m_fonts.at(id); }

const sf::SoundBuffer& resourceManager::getSoundBuffer(const std::string& id) { return m_soundBuffers.at(id); }

void resourceManager::clear()
{
    m_textures.clear();
    m_fonts.clear();
    m_soundBuffers.clear();
}