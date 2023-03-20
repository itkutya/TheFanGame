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

const std::any Keyboard::input() const noexcept
{
	return sf::Keyboard::isKeyPressed(this->m_KeyCode);
}

const std::any MouseButton::input() const noexcept
{
	return sf::Mouse::isButtonPressed(this->m_MouseButton);
}

const std::any MouseWheel::input() const noexcept
{
	return this->m_MouseWheel == this->m_Event.mouseWheelScroll.wheel ? this->m_Event.mouseWheelScroll.delta : 0.f;
}

const std::any JoystickButton::input() const noexcept
{
	return sf::Joystick::isButtonPressed(this->m_Identification, this->m_joystickButton);
}

const std::any JoystickAxis::input() const noexcept
{
	return sf::Joystick::getAxisPosition(this->m_Identification, this->m_JoystickAxis);
}