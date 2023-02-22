#pragma once

#include <string>

class Account
{
public:
    Account(Account const&) = delete;
    void operator=(Account const&) = delete;
    virtual ~Account() noexcept = default;

    [[nodiscard]] static Account& getInstance();

    std::string m_nickname;
    std::string m_password;
    std::string m_email;
    std::uint64_t m_lvl = 0;
    std::uint64_t m_lvlcap = 100;
    bool m_rememberme = false;

    void Login() noexcept;
    void Register() noexcept;
private:
    explicit Account() noexcept = default;
};

