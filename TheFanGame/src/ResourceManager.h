#pragma once

#include <variant>
#include <memory>
#include <string>

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class ResourceManager
{
    typedef std::variant<std::shared_ptr<sf::Sprite>,
                         std::shared_ptr<sf::Texture>,
                         std::shared_ptr<sf::Font>,
                         std::shared_ptr<sf::SoundBuffer>,
                         std::shared_ptr<sf::Music>>
    Resources;

public:
    ResourceManager(ResourceManager const&) = delete;
    void operator=(ResourceManager const&) = delete;
    virtual ~ResourceManager() noexcept = default;

    static ResourceManager& getInstance();
    
    template<class T> std::shared_ptr<T>& get(const std::string& id) noexcept;
    template<class T> std::shared_ptr<T>& add(const std::string& id) noexcept;
private:
    explicit ResourceManager() noexcept = default;
    std::unordered_map<std::string, Resources> m_resources;
};

template<class T>
inline std::shared_ptr<T>& ResourceManager::add(const std::string& id) noexcept
{
    this->m_resources[id] = std::make_shared<T>();
    return std::get<std::shared_ptr<T>>(this->m_resources[id]);
}

template<class T>
inline std::shared_ptr<T>& ResourceManager::get(const std::string& id) noexcept
{
    return std::get<std::shared_ptr<T>>(this->m_resources.at(id));
}