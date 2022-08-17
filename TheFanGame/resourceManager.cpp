#include "resourceManager.h"

std::unordered_map<std::string, std::variant<sf::Texture, sf::Font, sf::SoundBuffer>> resourceManager::m_resources;