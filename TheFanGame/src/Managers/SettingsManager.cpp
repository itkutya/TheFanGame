#include "SettingsManager.h"

SettingsManager& SettingsManager::getInstance(const char* path)
{
	static SettingsManager instance(path);
	return instance;
}

const bool SettingsManager::save(const char* path) noexcept
{
	FileManager fm;
	return fm.saveToFile(path, this->m_settings);
}

SettingsManager::SettingsManager(const char* path) noexcept
{
	if (this->m_first && this->load(path))
		this->m_first = false;
}

const bool SettingsManager::load(const char* path) noexcept
{
	FileManager fm(path);
	const auto& parse = fm.parseFile();
	for (const auto& mainchannel : parse)
	{
		for (const auto& settings : mainchannel.second)
		{
			const auto& [type, name, value] = settings;
			auto& mainbranch = this->m_settings.emplace(mainchannel.first, std::unordered_map<std::string, Setting>()).first->second;
			auto& newdata = mainbranch.emplace(name, Setting(type.c_str())).first->second;
			if (newdata.type == "int")
				newdata.value.m_int = std::stoi(value.data());
			else if (newdata.type == "bool")
				newdata.value.m_bool = std::stoi(value.data());
			else if (newdata.type == "string")
				newdata.value.m_string = value;
			else if (newdata.type == "u32")
				newdata.value.m_u32 = std::stoul(value.data());
			else if (newdata.type == "u64")
				newdata.value.m_u64 = std::stoull(value.data());
			else if (newdata.type == "Keyboard")
				newdata.value.m_input = std::make_unique<Keyboard>(StringToScanCode(value));
			else if (newdata.type == "MouseButton")
				newdata.value.m_input = std::make_unique<MouseButton>(StringToMouseButton(value));
			else if (newdata.type == "MouseWheel")
				newdata.value.m_input = std::make_unique<MouseWheel>(StringToMouseWheel(value));
			else if (newdata.type == "JoystickButton")
				newdata.value.m_input = std::make_unique<JoystickButton>(StringToJoystickButton(value));
			else if (newdata.type == "JoystickAxis")
				newdata.value.m_input = std::make_unique<JoystickAxis>(StringToJoystickAxis(value));
		}
	}
	return true;
}