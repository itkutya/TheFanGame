#pragma once

#include <variant>
#include <memory>
#include <string>

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class ResourceManager
{
public:
    struct Entity
    {
        sf::Sprite Sprite;
        sf::Texture Texture;
    };
    typedef std::variant<std::shared_ptr<sf::Sprite>,
                         std::shared_ptr<sf::Texture>,
                         std::shared_ptr<sf::Font>,
                         std::shared_ptr<sf::SoundBuffer>,
                         std::shared_ptr<sf::Music>,
                         std::shared_ptr<Entity>>
    Resources;

    ResourceManager(ResourceManager const&) = delete;
    void operator=(ResourceManager const&) = delete;
    virtual ~ResourceManager() noexcept = default;

    [[nodiscard]] static ResourceManager& getInstance();
    
    template<class T> std::shared_ptr<T>& add(const std::string& id) noexcept;
    template<class T> [[nodiscard]] bool load(const std::string& id, const std::string& path) noexcept;
    template<class T> bool remove(const std::string& id) noexcept;
    template<class T> [[nodiscard]] std::shared_ptr<T>& get(const std::string& id) noexcept;
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
inline bool ResourceManager::load(const std::string& id, const std::string& path) noexcept
{
    if constexpr (requires { T().loadFromFile(path); })
    {
        if (std::get<std::shared_ptr<T>>(this->m_resources.at(id))->loadFromFile(path))
            return true;
    }
    else if constexpr (requires { T().openFromFile(path); })
    {
        if (std::get<std::shared_ptr<T>>(this->m_resources.at(id))->openFromFile(path))
            return true;
    }
    return false;
}

template<class T>
inline bool ResourceManager::remove(const std::string& id) noexcept
{
    if (std::get<std::shared_ptr<T>>(this->m_resources.at(id)).use_count() == 1)
    {
        std::get<std::shared_ptr<T>>(this->m_resources.at(id)).reset();
        return true;
    }
    return false;
}

template<class T>
inline std::shared_ptr<T>& ResourceManager::get(const std::string& id) noexcept
{
    return std::get<std::shared_ptr<T>>(this->m_resources.at(id));
}