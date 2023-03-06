#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include "Account/Account.h"

//TODO:
//Rethink this
//Maybie unordered_map?
//IDK...
class FileManager
{
public:
    FileManager(FileManager const&) = delete;
    void operator=(FileManager const&) = delete;
    virtual ~FileManager() noexcept = default;

    [[nodiscard]] static FileManager& getInstance();

    [[nodiscard]] std::vector<std::string> load(const std::string& path, const char& seperator = '\n') noexcept;
    void save(const std::string& path, const std::vector<std::string>& elements) noexcept;
private:
    explicit FileManager() noexcept = default;

    Account* s_Account = &Account::getInstance();
};