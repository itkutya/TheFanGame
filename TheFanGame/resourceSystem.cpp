#include "resourceSystem.h"

std::unordered_map<std::string, std::variant<sf::Texture, sf::Font, sf::SoundBuffer, sf::MyMusic>> resourceSystem::m_resources;