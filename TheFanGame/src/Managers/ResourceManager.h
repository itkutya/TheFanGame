#pragma once

#include <variant>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class ResourceManager
{
public:
    struct Object
    {
        sf::Sprite Sprite;
        sf::Texture Texture;
    };
    struct AudioObject
    {
        sf::SoundBuffer Buffer;
        sf::Sound Sound;
    };
    typedef std::variant<std::unique_ptr<sf::Texture>,
                         std::unique_ptr<sf::Font>,
                         std::unique_ptr<sf::Music>,
                         std::unique_ptr<Object>,
                         std::unique_ptr<AudioObject>>
    Resources;

    ResourceManager(ResourceManager const&) = delete;
    void operator=(ResourceManager const&) = delete;
    virtual ~ResourceManager() noexcept = default;

    [[nodiscard]] static ResourceManager& getInstance();
    
    template<class T> T* add(const std::string_view id) noexcept;
    template<class T> [[nodiscard]] bool load(const std::string_view id, const char* path) noexcept;
    template<class T> [[nodiscard]] bool remove(const std::string_view id) noexcept;
    template<class T> [[nodiscard]] T* get(const std::string_view id) noexcept;
private:
    ResourceManager() noexcept = default;
    std::unordered_map<std::string_view, Resources> m_resources;
};

template<class T>
inline T* ResourceManager::add(const std::string_view id) noexcept
{
    this->m_resources[id] = std::make_unique<T>();
    return std::get<std::unique_ptr<T>>(this->m_resources.at(id)).get();
}

template<class T>
inline bool ResourceManager::load(const std::string_view id, const char* path) noexcept
{
    if constexpr (requires { T().loadFromFile(path); })
    {
        if (std::get<std::unique_ptr<T>>(this->m_resources.at(id))->loadFromFile(path))
            return true;
    }
    else if constexpr (requires { T().openFromFile(path); })
    {
        if (std::get<std::unique_ptr<T>>(this->m_resources.at(id))->openFromFile(path))
            return true;
    }
    return false;
}

template<class T>
inline bool ResourceManager::remove(const std::string_view id) noexcept
{
    if (std::get<std::unique_ptr<T>>(this->m_resources.at(id)) != nullptr)
    {
        std::get<std::unique_ptr<T>>(this->m_resources.at(id)).reset();
        this->m_resources.erase(id);
        return true;
    }
    return false;
}

template<class T>
inline T* ResourceManager::get(const std::string_view id) noexcept
{
    return std::get<std::unique_ptr<T>>(this->m_resources.at(id)).get();
}