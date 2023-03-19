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

bool InputManager::input(const Input& input) noexcept
{
	return input.input();
}

const bool Keyboard::input() const noexcept
{
	if (sf::Keyboard::isKeyPressed(this->m_KeyCode))
		return true;
	return false;
}

const bool Mouse::input() const noexcept
{
	if (sf::Mouse::isButtonPressed(this->m_MouseButton))
		return true;
	if (this->m_Event.mouseWheelScroll.wheel == this->m_MouseWheel)
		return true;
	if (this->m_Event.mouseWheelScroll.delta) //return float
		return true;
	return false;
}

const bool Joystick::input() const noexcept
{
	if (sf::Joystick::getAxisPosition(this->m_Identification, this->m_JoystickAxis)) //return float
		return true;
	if (sf::Joystick::isButtonPressed(this->m_Identification, this->m_joystickButton))
		return true;
	if (this->m_Event.joystickMove.position) //return float
		return true;
	if (this->m_Event.joystickMove.axis == this->m_JoystickAxis)
		return true;
	if (this->m_Event.joystickButton.button == this->m_joystickButton)
		return true;
	return false;
}