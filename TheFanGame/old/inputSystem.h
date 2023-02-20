#pragma once

#include "includes.h"

enum class Input_
{
	Left, Right, Forward, Backward, Zoom, Screenshot, Fire, Joystick_Fire
};

enum class InputType
{
	KeyboardInput,
	MouseInput,
	JoystickButtonInput,
	JoystickAxisInput
};

struct Input
{
	InputType m_InputType;
	sf::Event::EventType m_EventType;
	sf::Keyboard::Key m_KeyCode;
	sf::Mouse::Button m_MouseButton;
	sf::Joystick::Axis m_JoystickAxis;
	std::uint32_t m_joystickButton;
};

class inputSystem
{
public:
	inputSystem() noexcept;
	inputSystem(const inputSystem&) = delete;
	inputSystem(const inputSystem&&) = delete;
    inputSystem& operator=(inputSystem& other) = delete;
    inputSystem& operator=(const inputSystem& other) = delete;
	virtual ~inputSystem() = default;

	[[nodiscard]] const bool loadInput(const std::string& filePath) noexcept;
	[[nodiscard]] const bool saveInput(const std::string& filePath) const noexcept;

	auto& getInputHandler() const noexcept { return inputSystem::m_Action; }
	const bool checkForInput(const Input_& action) const noexcept;
	const bool checkForInput(const Input_& action, sf::Event& event) const noexcept;

	const std::string keyToString(const Input& it) const noexcept;
	const std::string eventToString(const Input& it) const noexcept;
	
    const std::optional<Input> checkForAnyKeyboardInput() const noexcept;
	const std::optional<Input> checkForAnyMouseInput() const noexcept;
	const std::optional<Input> checkForAnyJoystickInput() const noexcept;
	[[nodiscard]] const bool checkForAnyInput() const noexcept;
private:
	bool m_isAnyJoystickConnected = false;
	static inline std::array<std::pair<std::string, Input>, 8> m_Action;
};