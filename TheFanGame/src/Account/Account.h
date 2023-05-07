#pragma once

#include <functional>

#include "SFML/Network.hpp"
#include "imgui.h"
#include "imgui_stdlib.h"

#include "Managers/Setting/SettingsManager.h"
#include "Experience.h"

constexpr std::uint8_t MAX_USERNAME_LENGTH = 16;

template<class T>
concept Hashable = requires(T a)
{
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

class Account : NonCopyable
{
    SettingsManager& s_SettingsManager = SettingsManager::getInstance("Settings.ini");
public:
    virtual ~Account() noexcept = default;

    [[nodiscard]] static Account& getInstance();

    const bool Login(bool loaduplogin = false) noexcept;
    const bool Register() noexcept;

    std::string& m_username = this->s_SettingsManager["Account"]["Username"];
    std::string m_password;
    std::string m_email;
    bool& m_rememberme = this->s_SettingsManager["Account"]["RememberMe"];
    Experience m_experience;
    std::uint64_t m_currency = 0;
private:
    Account() noexcept = default;

    template<Hashable T> [[nodiscard]] const std::size_t CreateHashNumber(T& type) const noexcept;

    std::size_t& m_random = this->s_SettingsManager["Account"]["Random"];
};

template<Hashable T>
inline const std::size_t Account::CreateHashNumber(T& type) const noexcept
{
    std::hash<T> temp;
    return temp(type);
}