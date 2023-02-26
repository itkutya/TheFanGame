#pragma once

#include <fstream>
#include <vector>
#include <sstream>

#include "Account.h"

class FileManager
{
public:
    FileManager(FileManager const&) = delete;
    void operator=(FileManager const&) = delete;
    virtual ~FileManager() noexcept = default;

    [[nodiscard]] static FileManager& getInstance();

    //TODO:
    //Optional???
    //Make std::move not optional
    [[nodiscard]] std::vector<std::string> load(const std::string& path) noexcept;
    [[nodiscard]] std::vector<std::string> load(std::stringstream&& ss, const char& seperator) noexcept;
    void save(const std::string& path) noexcept;
private:
    explicit FileManager() noexcept = default;

    Account* s_Account = &Account::getInstance();
};