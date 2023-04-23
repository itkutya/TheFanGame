#include "InputManager.h"

InputManager& InputManager::getInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::processEvent(sf::Event& event) noexcept
{
	if (event.type == sf::Event::JoystickConnected)
		this->m_ConnectedJoystics.emplace_back(event.joystickConnect.joystickId);
	if (event.type == sf::Event::JoystickDisconnected)
		for (auto it = this->m_ConnectedJoystics.begin(); it != this->m_ConnectedJoystics.end(); ++it)
			if (*it == event.joystickConnect.joystickId)
				this->m_ConnectedJoystics.erase(it);
}

const bool InputManager::input(const Input& input) noexcept
{
	return input.input();
}

const float InputManager::input(const Input& input, const std::uint32_t j_id) noexcept
{
	return input.input(j_id);
}

const float InputManager::input(const Input& input, sf::Event& event) noexcept
{
	return input.input(event);
}

const bool InputManager::input(const std::string& id) noexcept
{
	return this->m_inputs.at(id)->input();
}

const float InputManager::input(const std::string& id, const std::uint32_t j_id) noexcept
{
	return this->m_inputs.at(id)->input(j_id);
}

const float InputManager::input(const std::string& id, sf::Event& event) noexcept
{
	return this->m_inputs.at(id)->input(event);
}

const std::string InputManager::inputToString(Input* input) noexcept
{
	if (input->m_type == InputType::Keyboard)
		return ScanCodeToString(static_cast<Keyboard*>(input)->m_KeyCode);
	else if (input->m_type == InputType::MouseButton)
		return MouseButtonToString(static_cast<MouseButton*>(input)->m_MouseButton);
	else if (input->m_type == InputType::MouseWheel)
		return MouseWheelToString(static_cast<MouseWheel*>(input)->m_MouseWheel);
	else if (input->m_type == InputType::JoystickButton)
		return JoystickButtonToString(static_cast<JoystickButton*>(input)->m_joystickButton);
	else if (input->m_type == InputType::JoystickAxis)
		return JoystickAxisToString(static_cast<JoystickAxis*>(input)->m_JoystickAxis);
	return "";
}

const std::unique_ptr<Input> InputManager::getAnyInput(sf::Event& event) noexcept
{
	if (event.type == sf::Event::KeyPressed)
		return std::make_unique<Keyboard>(event.key.scancode);
	else if(event.type == sf::Event::MouseButtonPressed)
		return std::make_unique<MouseButton>(event.mouseButton.button);
	else if (event.type == sf::Event::MouseWheelScrolled)
		return std::make_unique<MouseWheel>(event.mouseWheelScroll.wheel);
	else if (event.type == sf::Event::JoystickButtonPressed)
		return std::make_unique<JoystickButton>(event.joystickButton.button);
	else if (event.type == sf::Event::JoystickMoved)
		return std::make_unique<JoystickAxis>(event.joystickMove.axis);
	return std::make_unique<Input>();
}