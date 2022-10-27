#pragma once

#include <unordered_map>
#include <string>
#include <variant>
#include <stdlib.h>
#include <stdio.h>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Graphics.hpp"
		#include "SFML64/Audio.hpp"
	#else
		#include "SFML32/Graphics.hpp"
		#include "SFML32/Audio.hpp"
	#endif
#endif

class resourceSystem
{
public:
	resourceSystem() = default;
	resourceSystem(const resourceSystem&) = delete;
	resourceSystem(const resourceSystem&&) = delete;
	resourceSystem& operator=(resourceSystem& other) = delete;
	resourceSystem& operator=(const resourceSystem& other) = delete;
	virtual ~resourceSystem() { this->m_resources.clear(); };

	template<class T>
	inline const T& add(const std::string& id, const std::string& filePath)
	{
		this->m_resources[id].emplace<T>();
		if constexpr (requires { std::get<T>(this->m_resources[id]).loadFromFile(filePath); })
		{
			if (!std::get<T>(this->m_resources[id]).loadFromFile(filePath))
				std::printf("Cannot load from file: %s", filePath.c_str());
		}
		else
		{
			if (!std::get<T>(this->m_resources[id]).openFromFile(filePath))
				std::printf("Cannot open from file: %s", filePath.c_str());
		}
		return std::get<T>(this->m_resources[id]);
	};

	void release(const std::string& id) noexcept { this->m_resources.erase(id); };

	template<class T>
	[[nodiscard]] inline const T& c_get(const std::string& id) { return std::get<T>(this->m_resources.at(id)); };

	template<class T>
	[[nodiscard]] inline T& get(const std::string& id) { return std::get<T>(this->m_resources.at(id)); };
private:
	std::unordered_map<std::string, std::variant<sf::Texture, sf::Font, sf::SoundBuffer, sf::Music>> m_resources;
};