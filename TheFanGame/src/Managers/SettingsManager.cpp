#include "SettingsManager.h"

SettingsManager::~SettingsManager() noexcept
{
	for (auto& setting : this->m_settings)
		delete setting.second.value;
}

SettingsManager& SettingsManager::getInstance(const std::string& path)
{
	static SettingsManager instance(path);
	return instance;
}

bool SettingsManager::save(const std::string& path) noexcept
{
	bool succes = false;
	std::ofstream file(path);
	if (succes = file.is_open(); succes)
		for (auto& setting : this->m_settings)
		{
			std::string value;
			if(setting.second.type == "bool")
				value = std::to_string(*static_cast<bool*>(setting.second.value));
			else if (setting.second.type == "int")
				value = std::to_string(*static_cast<int*>(setting.second.value));
			else if (setting.second.type == "u32")
				value = std::to_string(*static_cast<std::uint64_t*>(setting.second.value));
			else if (setting.second.type == "u64")
				value = std::to_string(*static_cast<std::uint64_t*>(setting.second.value));
			else if (setting.second.type == "string")
				value = *static_cast<std::string*>(setting.second.value);
			file << setting.second.type << ':' << setting.first << ':' << value << '\n';
		}
	file.close();
	return succes;
}

SettingsManager::SettingsManager(const std::string& path) noexcept
{
	if (this->m_first && this->load(path))
		this->m_first = false;
}

bool SettingsManager::load(const std::string& path) noexcept
{
	bool success = false;
	std::ifstream file(path);
	std::string data;
	if (success = file.is_open(); success)
		while (std::getline(file, data))
		{
			std::vector<std::string> separate;
			std::stringstream temp(data);
			while (std::getline(temp, separate.emplace_back(), ':'));
			if (separate.size() >= 3)
				this->m_settings.insert({ separate[1], getTypeAndValue(separate) });
		}
	file.close();
	return success;
}

const SettingsManager::Setting SettingsManager::getTypeAndValue(const std::vector<std::string>& value) noexcept
{
	if (value[0] == "bool")
		return { "bool", new bool(value[2] == "1" ? true : false) };
	else if (value[0] == "int")
		return { "int", new int(std::stoi(value[2])) };
	else if (value[0] == "u32")
		return { "u32", new std::uint32_t(std::stoul(value[2])) };
	else if (value[0] == "u64")
		return { "u64", new std::uint64_t(std::stoull(value[2])) };
	else if (value[0] == "string")
		return { "string", new std::string(value[2]) };
	return { "string", new std::string(value[2]) };
}