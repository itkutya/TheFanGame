#pragma once

#include <fstream>
#include <vector>
#include <array>
#include <tuple>
#include <unordered_map>

#include "Utility.h"

class FileManager : NonCopyable
{
	typedef std::tuple<std::string, std::string, std::string> ParsedSetting;
public:
	FileManager() noexcept : m_filepath("") {};
	FileManager(const char* filepath) noexcept : m_filepath(filepath) { this->m_inputstream.open(this->m_filepath.data()); };
	~FileManager() noexcept { this->m_inputstream.close(); };

	const std::vector<std::pair<const std::string, std::vector<ParsedSetting>>> parseFile(const char* filepath) noexcept;
	const std::vector<std::pair<const std::string, std::vector<ParsedSetting>>> parseFile() noexcept;

	const bool saveToFile(const char* filepath, const std::unordered_map<std::string, std::unordered_map<std::string, Setting>>& data) noexcept;
private:
	std::string_view m_filepath;
	std::ifstream m_inputstream;

	const std::string trim(const std::string sw) noexcept;
};