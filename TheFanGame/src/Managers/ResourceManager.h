#pragma once

#include <variant>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Utility.h"

class ResourceManager : public Singleton<ResourceManager>
{
    friend class Singleton<ResourceManager>;
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
protected:
    ResourceManager() noexcept = default;
    ~ResourceManager() noexcept = default;
public:
    template<class T> T* add(const std::string_view id) noexcept;
    template<class T> void remove(const std::string_view id) noexcept;
    template<class T> [[nodiscard]] T* get(const std::string_view id) noexcept;
private:
    std::unordered_map<std::string_view, Resources> m_resources;
};

template<class T>
inline T* ResourceManager::add(const std::string_view id) noexcept
{
    this->m_resources[id] = std::make_unique<T>();
    return std::get<std::unique_ptr<T>>(this->m_resources.at(id)).get();
}

template<class T>
inline void ResourceManager::remove(const std::string_view id) noexcept
{
    this->m_resources.erase(id);
}

template<class T>
inline T* ResourceManager::get(const std::string_view id) noexcept
{
    return std::get<std::unique_ptr<T>>(this->m_resources.at(id)).get();
}