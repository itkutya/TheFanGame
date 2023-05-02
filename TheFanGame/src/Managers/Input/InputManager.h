#pragma once

#include "Managers/Setting/SettingsManager.h"

class InputManager : NonCopyable
{
	SettingsManager& s_SettingsManager = SettingsManager::getInstance();
public:
	InputManager() noexcept;
	~InputManager() noexcept = default;

	void setEvent(sf::Event& event);
	void processEvent(sf::Event& event) noexcept;

	[[nodiscard]] const Input::InputReturnType input(const std::string& id) noexcept;
	[[nodiscard]] const std::string inputToString(Input* input) noexcept;
	[[nodiscard]] std::unique_ptr<Input> getAnyInput(sf::Event& event) const noexcept;

	std::vector<std::uint32_t> m_ConnectedJoystics;
	std::unordered_map<std::string, Input*> m_inputs;
private:
	sf::Event m_event = sf::Event();
};