#pragma once

#include <string>
#include <functional>
#include <fstream>

#include "SFML/Network.hpp"
#include "imgui.h"
#include "imgui_stdlib.h"

#include "Managers/SettingsManager.h"
#include "Experience.h"

constexpr std::uint8_t MAX_USERNAME_LENGTH = 16;

template<class T>
concept Hashable = requires(T a)
{
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

class Account
{
    SettingsManager& s_Settings = SettingsManager::getInstance("Settings.ini");
public:
    Account(Account const&) = delete;
    void operator=(Account const&) = delete;
    virtual ~Account() noexcept = default;

    [[nodiscard]] static Account& getInstance();
    template<Hashable T> [[nodiscard]] const std::uint64_t CreateHashNumber(T& type) const noexcept;

    std::string& m_username = this->s_Settings[SettingsManager::FileNumber::USERNAME];
    std::string m_password;
    std::string m_email;
    bool m_rememberme = false;
    Experience m_experience;
    std::uint64_t m_random = 0;
private:
    explicit Account() noexcept = default;
};

template<Hashable T>
inline const std::uint64_t Account::CreateHashNumber(T& type) const noexcept
{
    std::hash<T> temp;
    return temp(type);
}