#include "SettingsManager.h"

SettingsManager& SettingsManager::getInstance(const std::string& path)
{
	static SettingsManager instance(path);
	return instance;
}

bool SettingsManager::save(const std::string& path) noexcept
{
	bool success = false;
	std::ofstream file(path, std::ios_base::trunc);
	if (success = file.is_open())
	{
		for (const auto& setting : this->m_settings)
		{
			file << std::string('[' + setting.first + ']' + '\n');
			for (const auto& value : this->m_settings[setting.first])
			{
				switch (value.second.type)
				{
				case SettingsManager::Setting::INT:
					file << this->convertTypeToString(value.second.type) << ' ' << value.first << ' ' << std::to_string(value.second.value.m_int) << '\n';
					break;
				case SettingsManager::Setting::BOOL:
					file << this->convertTypeToString(value.second.type) << ' ' << value.first << ' ' << std::to_string(value.second.value.m_bool) << '\n';
					break;
				case SettingsManager::Setting::STRING:
					file << this->convertTypeToString(value.second.type) << ' ' << value.first << ' ' << value.second.value.m_string << '\n';
					break;
				case SettingsManager::Setting::U32:
					file << this->convertTypeToString(value.second.type) << ' ' << value.first << ' ' << std::to_string(value.second.value.m_u32) << '\n';
					break;
				case SettingsManager::Setting::U64:
					file << this->convertTypeToString(value.second.type) << ' ' << value.first << ' ' << std::to_string(value.second.value.m_u64) << '\n';
					break;
				}
			}
		}
	}
	file.close();
	return success;
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
	if (success = file.is_open())
	{
		while (!file.eof())
		{
			std::string data;
			file >> data;
			if(data.length())
			{
				if (data[0] == '[' || data[data.length() - 1] == ']')
				{
					std::array<std::string, 3> setting;
					file >> setting[0] >> setting[1] >> setting[2];
					Setting::TYPE type = this->convertStrintToType(setting[0]);
					data = this->trim(data);
					switch (type)
					{
					case SettingsManager::Setting::INT:
						this->m_settings[data][setting[1]](type).value.m_int = std::stoi(setting[2]);
						break;
					case SettingsManager::Setting::BOOL:
						this->m_settings[data][setting[1]](type).value.m_bool = (setting[2] == "1" ? true : false);
						break;
					case SettingsManager::Setting::STRING:
						this->m_settings[data][setting[1]](type).value.m_string = std::string(setting[2]);
						break;
					case SettingsManager::Setting::U32:
						this->m_settings[data][setting[1]](type).value.m_u32 = std::stoul(setting[2]);
						break;
					case SettingsManager::Setting::U64:
						this->m_settings[data][setting[1]](type).value.m_u64 = std::stoull(setting[2]);
						break;
					}
				}
				else
				{
					std::array<std::string, 2> setting;
					file >> setting[0] >> setting[1];
					if (this->m_settings.size())
					{
						Setting::TYPE type = this->convertStrintToType(data);
						switch (type)
						{
						case SettingsManager::Setting::INT:
							(--this->m_settings.end())->second[setting[0]](type).value.m_int = std::stoi(setting[1]);
							break;
						case SettingsManager::Setting::BOOL:
							(--this->m_settings.end())->second[setting[0]](type).value.m_bool = (setting[1] == "1" ? true : false);
							break;
						case SettingsManager::Setting::STRING:
							(--this->m_settings.end())->second[setting[0]](type).value.m_string = std::string(setting[1]);
							break;
						case SettingsManager::Setting::U32:
							(--this->m_settings.end())->second[setting[0]](type).value.m_u32 = std::stoul(setting[1]);
							break;
						case SettingsManager::Setting::U64:
							(--this->m_settings.end())->second[setting[0]](type).value.m_u64 = std::stoull(setting[1]);
							break;
						}
					}
				}
			}
		}
	}
	file.close();
	return success;
}

const SettingsManager::Setting::TYPE SettingsManager::convertStrintToType(const std::string& type) noexcept
{
	if (type == "int")
		return SettingsManager::Setting::TYPE::INT;
	else if (type == "bool")
		return SettingsManager::Setting::TYPE::BOOL;
	else if (type == "string")
		return SettingsManager::Setting::TYPE::STRING;
	else if (type == "u32")
		return SettingsManager::Setting::TYPE::U32;
	else if (type == "u64")
		return SettingsManager::Setting::TYPE::U64;
	return SettingsManager::Setting::TYPE();
}

const std::string SettingsManager::trim(std::string& string) noexcept
{
	string.erase(string.begin());
	string.erase(string.end() - 1);
	return string;
}

const std::string SettingsManager::convertTypeToString(const Setting::TYPE& type) noexcept
{
	switch (type)
	{
	case SettingsManager::Setting::INT:
		return std::string("int");
	case SettingsManager::Setting::BOOL:
		return std::string("bool");
	case SettingsManager::Setting::STRING:
		return std::string("string");
	case SettingsManager::Setting::U32:
		return std::string("u32");
	case SettingsManager::Setting::U64:
		return std::string("u64");
	}
	return std::string();
}