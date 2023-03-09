#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <optional>

#include "Account/Account.h"

class FileManager
{
public:
    FileManager(FileManager const&) = delete;
    void operator=(FileManager const&) = delete;
    virtual ~FileManager() noexcept = default;

    [[nodiscard]] static FileManager& getInstance();

    [[nodiscard]] std::vector<std::string> load(const std::string& path, const char seperator = '\n') noexcept;
    [[nodiscard]] bool save(const std::string& path, const std::vector<std::string>& elements) noexcept;
private:
    explicit FileManager() noexcept = default;
};