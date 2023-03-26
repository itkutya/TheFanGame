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
				if (value.second.type == "int")
					file << value.second.type << ' ' << value.first << ' ' << std::to_string(value.second.value.m_int) << '\n';
				else if (value.second.type == "bool")
					file << value.second.type << ' ' << value.first << ' ' << std::to_string(value.second.value.m_bool) << '\n';
				else if (value.second.type == "string")
					file << value.second.type << ' ' << value.first << ' ' << value.second.value.m_string << '\n';
				else if (value.second.type == "u32")
					file << value.second.type << ' ' << value.first << ' ' << std::to_string(value.second.value.m_u32) << '\n';
				else if (value.second.type == "u64")
					file << value.second.type << ' ' << value.first << ' ' << std::to_string(value.second.value.m_u64) << '\n';
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
					this->m_settings.insert({ this->trim(data), {} });
				else
				{
					std::array<std::string, 2> setting;
					file >> setting[0] >> setting[1];
					if (this->m_settings.size())
					{
						if (data == "int")
						{
							const auto& newdata = (--this->m_settings.end())->second.insert({ setting[0], Setting("int") }).first;
							(*newdata).second.value.m_int = std::stoi(setting[1]);
						}
						else if(data == "bool")
						{
							const auto& newdata = (--this->m_settings.end())->second.insert({ setting[0], Setting("bool") }).first;
							(*newdata).second.value.m_bool = std::stoi(setting[1]);
						}
						else if (data == "string")
						{
							const auto& newdata = (--this->m_settings.end())->second.insert({ setting[0], Setting("string") }).first;
							(*newdata).second.value.m_string = std::string(setting[1]);
						}
						else if (data == "u32")
						{
							const auto& newdata = (--this->m_settings.end())->second.insert({ setting[0], Setting("u32") }).first;
							(*newdata).second.value.m_u32 = std::stoul(setting[1]);
						}
						else if (data == "u64")
						{
							const auto& newdata = (--this->m_settings.end())->second.insert({ setting[0], Setting("u64") }).first;
							(*newdata).second.value.m_u64 = std::stoull(setting[1]);
						}
					}
				}
			}
		}
	}
	file.close();
	return success;
}

const std::string SettingsManager::trim(std::string& string) noexcept
{
	string.erase(string.begin());
	string.erase(string.end() - 1);
	return string;
}