#pragma once

#include "InputTypes.h"
#include "Managers/SettingsManager.h"

class InputManager
{
	SettingsManager& s_SettingsManager = SettingsManager::getInstance();
public:
	InputManager(InputManager const&) = delete;
	void operator=(InputManager const&) = delete;
	virtual ~InputManager() noexcept = default;

	[[nodiscard]] static InputManager& getInstance();

	void processEvent(sf::Event& event) noexcept;

	[[nodiscard]] const bool input(const Input& input) noexcept;
	[[nodiscard]] const float input(const Input& input, const std::uint32_t j_id) noexcept;
	[[nodiscard]] const float input(const Input& input, sf::Event& event) noexcept;
	[[nodiscard]] const bool input(const std::string& id) noexcept;
	[[nodiscard]] const float input(const std::string& id, const std::uint32_t j_id) noexcept;
	[[nodiscard]] const float input(const std::string& id, sf::Event& event) noexcept;

	[[nodiscard]] const std::string inputToString(Input* input) noexcept;
	[[nodiscard]] const std::unique_ptr<Input> getAnyInput(sf::Event& event) noexcept;

	std::vector<std::uint32_t> m_ConnectedJoystics;
	std::unordered_map<std::string_view, Input*> m_inputs;
private:
	InputManager() noexcept
	{
		auto& input = this->s_SettingsManager["Input"];
		for (auto& i : input)
			this->m_inputs[i.first] = i.second.value.m_input.get();
	};
};