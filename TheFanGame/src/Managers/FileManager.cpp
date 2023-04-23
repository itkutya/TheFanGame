#include "FileManager.h"

const std::vector<std::pair<const std::string, std::vector<FileManager::ParsedSetting>>> FileManager::parseFile(const char* filepath) noexcept
{
	std::vector<std::pair<const std::string, std::vector<FileManager::ParsedSetting>>> parsedString;
	this->m_inputstream.open(filepath);
	if (this->m_inputstream.is_open())
	{
		while (!this->m_inputstream.eof())
		{
			std::string data;
			this->m_inputstream >> data;
			if (data.length())
			{
				if (data[0] == '[' || data[data.length() - 1] == ']')
					parsedString.emplace_back(std::make_pair<const std::string, std::vector<FileManager::ParsedSetting>>(this->trim(data), {}));
				else
				{
					std::array<std::string, 2> setting;
					this->m_inputstream >> setting[0] >> setting[1];
					if (parsedString.size())
						parsedString.back().second.emplace_back(data, setting[0].c_str(), setting[1].c_str());
				}
			}
		}
	}
	return parsedString;
}

const std::vector<std::pair<const std::string, std::vector<FileManager::ParsedSetting>>> FileManager::parseFile() noexcept
{
	std::vector<std::pair<const std::string, std::vector<FileManager::ParsedSetting>>> parsedString;
	if (this->m_inputstream.is_open())
	{
		while (!this->m_inputstream.eof())
		{
			std::string data;
			this->m_inputstream >> data;
			if (data.length())
			{
				if (data[0] == '[' || data[data.length() - 1] == ']')
					parsedString.emplace_back(std::make_pair<const std::string, std::vector<FileManager::ParsedSetting>>(this->trim(data.c_str()), {}));
				else
				{
					std::array<std::string, 2> setting;
					this->m_inputstream >> setting[0] >> setting[1];
					if (parsedString.size())
						parsedString.back().second.emplace_back(data.c_str(), setting[0].c_str(), setting[1].c_str());
				}
			}
		}
	}
	return parsedString;
}

const bool FileManager::saveToFile(const char* filepath, const std::unordered_map<std::string, std::unordered_map<std::string, Setting>>& data) noexcept
{
	std::ofstream file(filepath, std::ios_base::trunc);
	if (file.is_open())
	{
		for (const auto& settings : data)
		{
			file << '[' << settings.first << ']' << '\n';
			for (const auto& setting : data.at(settings.first))
			{
				const auto& [name, v] = setting;
				const auto& [type, value] = v;
				file << type << ' ' << name << ' ';
				if (type == "int")
					file << std::to_string(value.m_int) << '\n';
				else if (type == "bool")
					file << std::to_string(value.m_bool) << '\n';
				else if (type == "string")
					file << value.m_string << '\n';
				else if (type == "u32")
					file << std::to_string(value.m_u32) << '\n';
				else if (type == "u64")
					file << std::to_string(value.m_u64) << '\n';
				else if (type == "Keyboard")
					file << ScanCodeToString(static_cast<Keyboard*>(value.m_input.get())->m_KeyCode) << '\n';
				else if (type == "MouseButton")
					file << MouseButtonToString(static_cast<MouseButton*>(value.m_input.get())->m_MouseButton) << '\n';
				else if (type == "MouseWheel")
					file << MouseWheelToString(static_cast<MouseWheel*>(value.m_input.get())->m_MouseWheel) << '\n';
				else if (type == "JoystickButton")
					file << JoystickButtonToString(static_cast<JoystickButton*>(value.m_input.get())->m_joystickButton) << '\n';
				else if (type == "JoystickAxis")
					file << JoystickAxisToString(static_cast<JoystickAxis*>(value.m_input.get())->m_JoystickAxis) << '\n';
			}
		}
		file.close();
		return true;
	}
	file.close();
	return false;
}

const std::string FileManager::trim(const std::string sw) noexcept
{
	return { sw.begin() + 1, sw.end() - 1 };
}