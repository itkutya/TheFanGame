#include "SettingsManager.h"

std::any& SettingsManager::operator[](const std::string& id) noexcept
{
	return this->m_settings[id];
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
			try
			{
				bool* boolean = std::any_cast<bool>(&setting.second);
				if (boolean)
					file << this->getUnderLyingType(setting.second) << ':' << setting.first << ':' << *boolean << '\n';

				int* i = std::any_cast<int>(&setting.second);
				if (i)
					file << this->getUnderLyingType(setting.second) << ':' << setting.first << ':' << *i << '\n';

				std::uint32_t* u32 = std::any_cast<std::uint32_t>(&setting.second);
				if (u32)
					file << this->getUnderLyingType(setting.second) << ':' << setting.first << ':' << *u32 << '\n';

				std::uint64_t* u64 = std::any_cast<std::uint64_t>(&setting.second);
				if (u64)
					file << this->getUnderLyingType(setting.second) << ':' << setting.first << ':' << *u64 << '\n';

				std::string* string = std::any_cast<std::string>(&setting.second);
				if (string)
					file << this->getUnderLyingType(setting.second) << ':' << setting.first << ':' << *string << '\n';
			}
			catch (const std::exception&)
			{
				std::printf("unhandeled std::any type...\n");
			}
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

const std::any SettingsManager::getTypeAndValue(const std::vector<std::string>& value) noexcept
{
	if (value[0] == "bool")
		return std::any_cast<bool>(value[2] == "1" ? true : false);
	else if (value[0] == "int")
		return std::any_cast<int>(std::stoi(value[2]));
	else if (value[0] == "u32")
		return std::any_cast<std::uint32_t>(std::stoul(value[2]));
	else if (value[0] == "u64")
		return std::any_cast<std::uint64_t>(std::stoull(value[2]));
	else if (value[0] == "string")
		return std::any_cast<std::string>(value[2]);
	return std::any_cast<std::string>(value[2]);
}

const std::string SettingsManager::getUnderLyingType(std::any& value) noexcept
{
	if (std::string(value.type().name()).find("string") != std::string::npos)
		return "string";
	if (std::string(value.type().name()).find("64") != std::string::npos)
		return "u64";
	if (std::string(value.type().name()).find("32") != std::string::npos)
		return "u32";
	if (std::string(value.type().name()).find("int") != std::string::npos)
		return "int";
	if (std::string(value.type().name()).find("bool") != std::string::npos)
		return "bool";
	return std::string();
}