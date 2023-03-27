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

const std::string InputManager::inputToString(std::shared_ptr<Input>& input) noexcept
{
	if (input->m_type == InputType::Keyboard)
		return ScanCodeToString(static_cast<Keyboard*>(input.get())->m_KeyCode);
	else if (input->m_type == InputType::MouseButton)
		return MouseButtonToString(static_cast<MouseButton*>(input.get())->m_MouseButton);
	else if (input->m_type == InputType::MouseWheel)
		return MouseWheelToString(static_cast<MouseWheel*>(input.get())->m_MouseWheel);
	else if (input->m_type == InputType::JoystickButton)
		return JoystickButtonToString(static_cast<JoystickButton*>(input.get())->m_joystickButton);
	else if (input->m_type == InputType::JoystickAxis)
		return JoystickAxisToString(static_cast<JoystickAxis*>(input.get())->m_JoystickAxis);
	return "";
}