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
		std::unordered_map<std::string, std::unordered_map<std::string, Setting>>::iterator last;
		while (!file.eof())
		{
			std::string data;
			file >> data;
			if (data.length())
			{
				if (data[0] == '[' || data[data.length() - 1] == ']')
					last = this->m_settings.insert({ this->trim(data), {} }).first;
				else
				{
					std::array<std::string, 2> setting;
					file >> setting[0] >> setting[1];
					if (this->m_settings.size())
					{
						if (data == "int")
						{
							const auto& newdata = (last)->second.insert({ setting[0], Setting("int") }).first;
							(*newdata).second.value.m_int = std::stoi(setting[1]);
						}
						else if (data == "bool")
						{
							const auto& newdata = (last)->second.insert({ setting[0], Setting("bool") }).first;
							(*newdata).second.value.m_bool = std::stoi(setting[1]);
						}
						else if (data == "string")
						{
							const auto& newdata = (last)->second.insert({ setting[0], Setting("string") }).first;
							(*newdata).second.value.m_string = std::string(setting[1]);
						}
						else if (data == "u32")
						{
							const auto& newdata = (last)->second.insert({ setting[0], Setting("u32") }).first;
							(*newdata).second.value.m_u32 = std::stoul(setting[1]);
						}
						else if (data == "u64")
						{
							const auto& newdata = (last)->second.insert({ setting[0], Setting("u64") }).first;
							(*newdata).second.value.m_u64 = std::stoull(setting[1]);
						}
						else if (data == "Keyboard")
						{
							const auto& newdata = (last)->second.insert({ setting[0], Setting("Keyboard") }).first;
							(*newdata).second.value.m_input = std::make_unique<Keyboard>(StringToScanCode(setting[1]));
						}
						else if (data == "MouseButton")
						{
							const auto& newdata = (last)->second.insert({ setting[0], Setting("MouseButton") }).first;
							(*newdata).second.value.m_input = std::make_unique<MouseButton>(StringToMouseButton(setting[1]));
						}
						else if (data == "MouseWheel")
						{
							const auto& newdata = (last)->second.insert({ setting[0], Setting("MouseWheel") }).first;
							(*newdata).second.value.m_input = std::make_unique<MouseWheel>(StringToMouseWheel(setting[1]));
						}
						else if (data == "JoystickButton")
						{
							const auto& newdata = (last)->second.insert({ setting[0], Setting("JoystickButton") }).first;
							(*newdata).second.value.m_input = std::make_unique<JoystickButton>(StringToJoystickButton(setting[1]));
						}
						else if (data == "JoystickAxis")
						{
							const auto& newdata = (last)->second.insert({ setting[0], Setting("JoystickAxis") }).first;
							(*newdata).second.value.m_input = std::make_unique<JoystickAxis>(StringToJoystickAxis(setting[1]));
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