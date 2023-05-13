#include "CharactersManager.h"

CharactersManager::CharactersManager() noexcept
{
    Database db("http://thefangamedb.000webhostapp.com", "/characters.php", sf::Http::Request::Method::Get, sf::seconds(3));
    if(db.getStatus() != sf::Http::Response::Status::Ok)
        ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured!"));
    else
    {
        enum CharData { SUCCESS = 0, NAME, ATK, DEF };
        if (db.getData(CharData::SUCCESS) != "Success.")
            ImGui::InsertNotification(ImGuiToast(ImGuiToastType_Error, 3000, "Error occured while loading in the characters!"));
        else
        {
            for (std::size_t i = 0; i < db.getDataSize() - 2; i += 3)
            {
                this->m_Characters[db.getData(CharData::NAME + i)] = 
                Character(db.getData(CharData::NAME + i), std::stoul(db.getData(CharData::ATK + i)), std::stoul(db.getData(CharData::DEF + i)));
            }
        }
    }
}