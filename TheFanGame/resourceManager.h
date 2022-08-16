#pragma once

#include <unordered_map>
#include <string>
#include <variant>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/Graphics.hpp"
		#include "SFML64/Audio.hpp"
	#else
		#include "SFML32/Graphics.hpp"
		#include "SFML32/Audio.hpp"
	#endif
#endif

class resourceManager
{
public:
	resourceManager() = delete;
	resourceManager(const resourceManager&) = delete;
	virtual ~resourceManager() { clear(); };

	template<class T>
	static inline const void add(const std::string& id, const std::string& filePath)
	{
		m_resources.insert({ id, std::variant<sf::Texture, sf::Font, sf::SoundBuffer>() });
		m_resources.at(id) = T();

		if (!std::get<T>(m_resources.at(id)).loadFromFile(filePath))
			throw "Cannot load texture...\n";
	};

	template<class T>
	static inline const T& get(const std::string& id) { return std::get<T>(m_resources.at(id)); };

	static inline void clear() { m_resources.clear(); };
private:
	static std::unordered_map<std::string, std::variant<sf::Texture, sf::Font, sf::SoundBuffer>> m_resources;
};