#pragma once

#include <string>
#include <unordered_map>
#include <fstream>

#include "imgui.h"
#include "imgui_stdlib.h"

#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"

#include "Utility.h"

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
    CharactersManager() noexcept
    {
        sf::Http http("http://thefangamedb.000webhostapp.com");
        sf::Http::Request request("/characters.php", sf::Http::Request::Method::Get);
        sf::Http::Response response = http.sendRequest(request, sf::seconds(3.f));

        if (response.getStatus() != sf::Http::Response::Status::Ok)
            ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
        else
        {
            enum CharData { SUCCESS = 0, NAME, ATK, DEF };
            std::printf("%s\n", response.getBody().c_str());

            std::vector<std::string> data;
            std::stringstream ss(response.getBody());
            while (std::getline(ss, data.emplace_back(), '#'));
            if (data[CharData::SUCCESS] != std::string("Success."))
                ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured while loading in the characters!"));
            else
                for (std::size_t i = 0; i < data.size() - 2; i += 3)
                    this->m_Characters[data[CharData::NAME + i]] = Character(
                                   data[CharData::NAME + i], 
                        std::stoul(data[CharData::ATK + i]), 
                        std::stoul(data[CharData::DEF + i]));
        }
    };
    ~CharactersManager() noexcept = default;
public:
    std::unordered_map<std::string, Character> m_Characters;
private:
};