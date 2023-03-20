#include "SettingsManager.h"

SettingsManager& SettingsManager::getInstance(const std::string& path)
{
	static SettingsManager instance(path);
	return instance;
}

bool SettingsManager::save(const std::string& path) noexcept
{
	bool succes = false;
	std::ofstream file(path, std::ios_base::trunc);
	if (succes = file.is_open(); succes)
		for (auto& setting : this->m_settings)
		{
			std::string value;
			switch (setting.second.type)
			{
			case SettingsManager::Setting::INT:
				value = std::to_string(setting.second.value.m_int);
				break;
			case SettingsManager::Setting::BOOL:
				value = std::to_string(setting.second.value.m_bool);
				break;
			case SettingsManager::Setting::STRING:
				value = setting.second.value.m_string;
				break;
			case SettingsManager::Setting::U32:
				value = std::to_string(setting.second.value.m_u32);
				break;
			case SettingsManager::Setting::U64:
				value = std::to_string(setting.second.value.m_u64);
				break;
			}
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
				this->m_settings.insert({ separate[1], this->getTypeAndValue(separate) });
		}
	file.close();
	return success;
}

const SettingsManager::Setting SettingsManager::getTypeAndValue(const std::vector<std::string>& value) noexcept
{
	Setting temp{};
	temp.type = static_cast<SettingsManager::Setting::TYPE>(std::stoi(value[0]));
	switch (temp.type)
	{
	case SettingsManager::Setting::INT:
		temp.value.m_int = std::stoi(value[2]);
		break;
	case SettingsManager::Setting::BOOL:
		temp.value.m_bool = (value[2] == "1" ? true : false);
		break;
	case SettingsManager::Setting::STRING:
		new (&temp.value.m_string) std::string(value[2]);
		break;
	case SettingsManager::Setting::U32:
		temp.value.m_u32 = std::stoul(value[2]);
		break;
	case SettingsManager::Setting::U64:
		temp.value.m_u64 = std::stoull(value[2]);
		break;
	}
	return temp;
}