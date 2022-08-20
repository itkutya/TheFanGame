#include "resourceManager.h"

std::unordered_map<std::string, std::variant<sf::Texture, sf::Font, sf::SoundBuffer, sf::MyMusic>> resourceManager::m_resources;