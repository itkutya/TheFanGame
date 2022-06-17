#include "resourceManager.h"

const void resourceManager::addTexture(const int& id, const std::string& filePath, const bool& wantRepeated)
{
    this->m_textures[id] = std::make_unique<sf::Texture>();

    if (!this->m_textures[id]->loadFromFile(filePath))
        throw "Cannot load texture...\n";
        
    this->m_textures[id]->setRepeated(wantRepeated);
}

const void resourceManager::addFont(const int& id, const std::string& filePath)
{
    this->m_fonts[id] = std::make_unique<sf::Font>();

    if (!m_fonts[id]->loadFromFile(filePath))
        throw "Cannot load font...\n";
}

const sf::Texture& resourceManager::getTexture(const int& id) const { return *(this->m_textures.at(id).get()); }

const sf::Font& resourceManager::getFont(const int& id) const { return *(this->m_fonts.at(id).get()); }