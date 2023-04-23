#include "SettingsManager.h"

SettingsManager& SettingsManager::getInstance(const char* path)
{
	static SettingsManager instance(path);
	return instance;
}

const bool SettingsManager::save(const char* path) noexcept
{
	bool success = false;
	/*
	std::ofstream file(path, std::ios_base::trunc);
	if (success = file.is_open())
	{
		for (const auto& setting : this->m_settings)
		{
			file << '[' << setting.first << ']' << '\n';
			for (const auto& value : this->m_settings[setting.first])
			{
				if (value.second.type == "int")
					file << value.second.type << ' ' << value.first << ' ' << std::to_string(value.second.value.m_int) << '\n';
				else if (value.second.type == "bool")
					file << value.second.type << ' ' << value.first << ' ' << std::to_string(value.second.value.m_bool) << '\n';
				else if (value.second.type == "string")
					file << value.second.type << ' ' << value.first << ' ' << value.second.value.m_string_view << '\n';
				else if (value.second.type == "u32")
					file << value.second.type << ' ' << value.first << ' ' << std::to_string(value.second.value.m_u32) << '\n';
				else if (value.second.type == "u64")
					file << value.second.type << ' ' << value.first << ' ' << std::to_string(value.second.value.m_u64) << '\n';
				else if (value.second.type == "Keyboard")
					file << value.second.type << ' ' << value.first << ' ' << ScanCodeToString(static_cast<Keyboard*>(value.second.value.m_input.get())->m_KeyCode) << '\n';
				else if (value.second.type == "MouseButton")
					file << value.second.type << ' ' << value.first << ' ' << MouseButtonToString(static_cast<MouseButton*>(value.second.value.m_input.get())->m_MouseButton) << '\n';
				else if (value.second.type == "MouseWheel")
					file << value.second.type << ' ' << value.first << ' ' << MouseWheelToString(static_cast<MouseWheel*>(value.second.value.m_input.get())->m_MouseWheel) << '\n';
				else if (value.second.type == "JoystickButton")
					file << value.second.type << ' ' << value.first << ' ' << JoystickButtonToString(static_cast<JoystickButton*>(value.second.value.m_input.get())->m_joystickButton) << '\n';
				else if (value.second.type == "JoystickAxis")
					file << value.second.type << ' ' << value.first << ' ' << JoystickAxisToString(static_cast<JoystickAxis*>(value.second.value.m_input.get())->m_JoystickAxis) << '\n';
			}
		}
	}
	file.close();
	*/
	return success;
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