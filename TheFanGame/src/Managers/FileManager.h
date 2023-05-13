#pragma once

#include <vector>
#include <tuple>
#include <unordered_map>

#include "Setting/Setting.h"

class FileManager
{
	typedef std::tuple<const std::string, const std::string, const std::string> ParsedSetting;
	typedef std::pair<const std::string, std::vector<ParsedSetting>> ParsedBranch;
public:
	FileManager() noexcept	= default;
	~FileManager() noexcept = default;

	const std::vector<ParsedBranch> parseFile(const char* filepath) noexcept;
	const bool saveToFile(const char* filepath, std::unordered_map<std::string, std::unordered_map<std::string, Setting>>& data) noexcept;
private:
	const std::string trim(const std::string s) noexcept;
};