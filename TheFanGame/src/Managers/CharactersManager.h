#pragma once

#include <unordered_map>

#include "imgui.h"
#include "imgui_stdlib.h"

#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"

#include "Utility.h"
#include "Managers/Network/Database.h"

class CharactersManager : public Singleton<CharactersManager>
{
    friend class Singleton<CharactersManager>;

    struct Character
    {
        Character() noexcept = default;
        Character(std::string name, std::uint32_t atk, std::uint32_t def) noexcept : m_Name(std::move(name)), m_ATK(atk), m_DEF(def) {};
        ~Character() noexcept = default;

        std::string m_Name = "";
        std::uint32_t m_ATK = 0;
        std::uint32_t m_DEF = 0;
        float m_Health = 0.f;
        float m_Speed = 0.f;
        bool m_Unlocked = false;
        sf::Texture m_Icon;
        std::uint64_t m_Price = 0;
    };
protected:
    CharactersManager() noexcept;
    ~CharactersManager() noexcept = default;
public:
    std::unordered_map<std::string, Character> m_Characters;
private:
};