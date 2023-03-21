#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <any>
#include <fstream>
#include <sstream>

#include "SFML/Window.hpp"

#include "SettingsManager.h"

enum class InputType
{
	None, Keyboard, MouseButton, MouseWheel, JoystickButton, JoystickAxis, Count
};

class Input
{
public:
	explicit Input() noexcept = default;
	virtual ~Input() noexcept = default;

	virtual const std::any input() const noexcept = 0;

	InputType m_type = InputType::None;
};

class Keyboard : public Input
{
public:
	explicit Keyboard(sf::Keyboard::Scancode key) noexcept : m_KeyCode(key) { this->m_type = InputType::Keyboard; };
	virtual ~Keyboard() noexcept = default;

	virtual const std::any input() const noexcept override;
private:
	sf::Keyboard::Scancode m_KeyCode = sf::Keyboard::Scancode();
};

class MouseButton : public Input
{
public:
	explicit MouseButton(sf::Mouse::Button button) noexcept : m_MouseButton(button) { this->m_type = InputType::MouseButton; };
	virtual ~MouseButton() noexcept = default;

	virtual const std::any input() const noexcept override;
private:
	sf::Mouse::Button m_MouseButton = sf::Mouse::Button();
};

class MouseWheel : public Input
{
public:
	explicit MouseWheel(sf::Mouse::Wheel wheel, sf::Event& event) noexcept : m_MouseWheel(wheel), m_Event(event) { this->m_type = InputType::MouseWheel; };
	virtual ~MouseWheel() noexcept = default;

	virtual const std::any input() const noexcept override;
private:
	sf::Mouse::Wheel m_MouseWheel = sf::Mouse::Wheel();
	sf::Event m_Event = sf::Event();
};

class JoystickButton : public Input
{
public:
	explicit JoystickButton(std::uint32_t id, std::uint32_t button) noexcept : m_Identification(id), m_joystickButton(button) { this->m_type = InputType::JoystickButton; };
	virtual ~JoystickButton() noexcept = default;

	virtual const std::any input() const noexcept override;
private:
	std::uint32_t m_Identification = 0;
	std::uint32_t m_joystickButton = 0;
};

class JoystickAxis : public Input
{
public:
	explicit JoystickAxis(std::uint32_t id, sf::Joystick::Axis axis) noexcept : m_Identification(id), m_JoystickAxis(axis) { this->m_type = InputType::JoystickAxis; };
	virtual ~JoystickAxis() noexcept = default;

	virtual const std::any input() const noexcept override;
private:
	std::uint32_t m_Identification = 0;
	sf::Joystick::Axis m_JoystickAxis = sf::Joystick::Axis();
};

class InputManager
{
	SettingsManager& s_SettingsManager = SettingsManager::getInstance();
public:
	InputManager(InputManager const&) = delete;
	void operator=(InputManager const&) = delete;
	virtual ~InputManager() noexcept = default;

	[[nodiscard]] static InputManager& getInstance();

	void processEvent(sf::Event& event) noexcept;

	template<class T> [[nodiscard]] const T input(const Input& input) noexcept;
	template<class T> [[nodiscard]] const T input(const std::string& id) noexcept;
private:
	explicit InputManager() noexcept
	{
		//load from inputsettings.ini...
		std::ifstream file("InputSettings.ini");
		std::string data;
		if (file.is_open())
			while (std::getline(file, data))
			{
				std::vector<std::string> separate;
				std::stringstream temp(data);
				while (std::getline(temp, separate.emplace_back(), ':'));
				if (separate.size() >= 3)
				{
					const InputType type = static_cast<const InputType>(std::stoi(separate[0]));
					switch (type)
					{
					case InputType::Keyboard:
						this->m_inputs[separate[1]] = std::make_unique<Keyboard>(static_cast<sf::Keyboard::Scancode>(std::stoi(separate[2])));
						break;
					case InputType::MouseButton:
						this->m_inputs[separate[1]] = std::make_unique<MouseButton>(static_cast<sf::Mouse::Button>(std::stoi(separate[2])));
						break;
					case InputType::MouseWheel:
						//this->m_inputs[separate[1]] = std::make_unique<MouseWheel>(static_cast<sf::Mouse::Wheel>(std::stoi(separate[2])));
						break;
					case InputType::JoystickButton:
						//this->m_inputs[separate[1]] = std::make_unique<JoystickButton>(std::stoi(separate[2]));
						break;
					case InputType::JoystickAxis:
						//this->m_inputs[separate[1]] = std::make_unique<JoystickAxis>(static_cast<sf::Joystick::Axis>(std::stoi(separate[2])));
						break;
					}
				}
			}
		file.close();
	};

	std::vector<std::uint32_t> m_ConnectedJoystics;
	std::unordered_map<std::string, std::unique_ptr<Input>> m_inputs;
};

template<class T>
inline const T InputManager::input(const Input& input) noexcept
{
	return std::any_cast<T>(input.input());
}

template<class T>
inline const T InputManager::input(const std::string& id) noexcept
{
	return std::any_cast<T>(this->m_inputs[id]->input());
}