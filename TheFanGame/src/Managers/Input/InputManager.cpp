#include "InputManager.h"

InputManager::InputManager() noexcept
{
	auto& input = this->s_SettingsManager["Input"];
	for (const auto& i : input)
		this->m_inputs[i.first] = i.second.value.m_input.get();
}

void InputManager::processEvent(sf::Event& event) noexcept
{
	this->m_event = &event;
	if (event.type == sf::Event::JoystickConnected)
		this->m_ConnectedJoystics.emplace_back(event.joystickConnect.joystickId);
	if (event.type == sf::Event::JoystickDisconnected)
		for (auto it = this->m_ConnectedJoystics.begin(); it != this->m_ConnectedJoystics.end(); ++it)
			if (*it == event.joystickConnect.joystickId)
				this->m_ConnectedJoystics.erase(it);
}

const float InputManager::input(const std::string& id) noexcept
{
	auto& input = this->m_inputs.at(id);
	if (input->m_type == InputType::Keyboard || input->m_type == InputType::MouseButton)
		return input->input();
	else if (input->m_type == InputType::MouseWheel)
		return input->input(*this->m_event);
	else if (input->m_type == InputType::JoystickButton || input->m_type == InputType::JoystickAxis)
		for (const auto& id : this->m_ConnectedJoystics)
			return input->input(id);
	return false;
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

std::unique_ptr<Input> InputManager::getAnyInput(sf::Event& event) const noexcept
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