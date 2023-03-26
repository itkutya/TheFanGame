#pragma once

#include "InputTypes.h"
#include "SettingsManager.h"

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
		auto& input = this->s_SettingsManager["Input"];
		for (auto& i : input)
			this->m_inputs[i.first] = i.second.value.m_input;
	};

	std::vector<std::uint32_t> m_ConnectedJoystics;
	std::unordered_map<std::string, std::shared_ptr<Input>> m_inputs;
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