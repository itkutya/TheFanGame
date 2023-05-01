#pragma once

#include <vector>
#include <tuple>

#include "Utility.h"
#include "Setting.h"
#include <unordered_map>

class FileManager : NonCopyable
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