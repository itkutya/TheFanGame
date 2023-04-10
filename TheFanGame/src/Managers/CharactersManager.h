#pragma once

#include <string_view>
#include <array>
#include <unordered_map>

#include "SFML/Graphics.hpp"

class CharactersManager
{
    class Character
    {
    public:
        Character() noexcept = default;
        virtual ~Character() noexcept = default;

        std::string_view m_Name = "";
        std::uint32_t m_ATK = 0;
        std::uint32_t m_DEF = 0;
        float m_Health = 0.f;
        float m_Speed = 0.f;
        bool m_Unlocked = false;
        sf::Texture m_Icon;
        std::uint64_t m_Price = 0;
    private:
    };

    class ShishiroBotan : public Character
    {
    public:
        ShishiroBotan() noexcept 
        { 
            this->m_Name = "Shishiro Botan";
            this->m_ATK = 100;
            this->m_DEF = 100;
            this->m_Health = 100.f;
            this->m_Speed = 100.f;
            this->m_Price = 100;
            if (!this->m_Icon.loadFromFile("Resources/Icons.png", sf::IntRect({ 0, 0 }, { 100, 100 })))
                std::printf("Could not load Shishiro Botan Icon'.");
        };
    private:
    };
public:
    CharactersManager(CharactersManager const&) = delete;
    void operator=(CharactersManager const&) = delete;
    virtual ~CharactersManager() noexcept = default;

    [[nodiscard]] static CharactersManager& getInstance();

    std::unordered_map<std::string_view, Character> m_Characters;
private:
    CharactersManager() noexcept
    {
        constexpr std::array names = { "Shishiro Botan", "Omaru Polka" };
        this->m_Characters[names[0]] = ShishiroBotan();
        this->m_Characters[names[1]] = {};
    };
};