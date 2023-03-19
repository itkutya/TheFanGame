#pragma once

#include <vector>

#include "SFML/Window.hpp"

#include "SettingsManager.h"

class Input
{
public:
	explicit Input() noexcept = default;
	virtual ~Input() noexcept = default;

	virtual const bool input() const noexcept = 0;
};

class Mouse : public Input
{
public:
	explicit Mouse(sf::Mouse::Button button) noexcept : m_MouseButton(button) {};
	explicit Mouse(sf::Mouse::Wheel wheel, sf::Event& event) noexcept : m_MouseWheel(wheel), m_Event(event) {};
	virtual ~Mouse() noexcept = default;

	virtual const bool input() const noexcept override;
private:
	sf::Mouse::Button m_MouseButton = sf::Mouse::Button();
	sf::Mouse::Wheel m_MouseWheel = sf::Mouse::Wheel();
	sf::Event m_Event = sf::Event();
};

class Keyboard : public Input
{
public:
	explicit Keyboard(sf::Keyboard::Scancode key) noexcept : m_KeyCode(key) {};
	virtual ~Keyboard() noexcept = default;

	virtual const bool input() const noexcept override;
private:
	sf::Keyboard::Scancode m_KeyCode = sf::Keyboard::Scancode();
};

class Joystick : public Input
{
public:
	explicit Joystick(std::uint32_t id, sf::Joystick::Axis axis, sf::Event& event) noexcept : m_Identification(id), m_JoystickAxis(axis), m_Event(event) {};
	explicit Joystick(std::uint32_t id, std::uint32_t button) noexcept : m_Identification(id), m_joystickButton(button) {};
	virtual ~Joystick() noexcept = default;

	virtual const bool input() const noexcept override;
private:
	std::uint32_t m_Identification = 0;
	std::uint32_t m_joystickButton = 0;
	sf::Joystick::Axis m_JoystickAxis = sf::Joystick::Axis();
	sf::Event m_Event = sf::Event();
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
	[[nodiscard]] bool input(const Input& input) noexcept;

	std::vector<std::uint32_t> m_ConnectedJoystics;
private:
	explicit InputManager() noexcept = default;
};