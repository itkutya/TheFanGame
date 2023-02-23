#pragma once

#include <random>
#include <string>

#include "SFML/Network.hpp"
#include "imgui.h"
#include "imgui_stdlib.h"

#include "Experience.h"

constexpr std::uint8_t MAX_USERNAME_LENGTH = 16;

class Account
{
public:
    Account(Account const&) = delete;
    void operator=(Account const&) = delete;
    virtual ~Account() noexcept = default;

    [[nodiscard]] static Account& getInstance();

    std::string m_username;
    std::string m_password;
    std::string m_email;
    bool m_rememberme = false;
    Experience m_Experience;

    bool Login() noexcept;
    bool Register() noexcept;
    void CreateRandomNumber() noexcept;
private:
    explicit Account() noexcept = default;
    std::uint64_t m_random = 0;
};