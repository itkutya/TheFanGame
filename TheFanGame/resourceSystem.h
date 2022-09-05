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

namespace sf
{
	class MyMusic : public sf::Music
	{
	public:
		MyMusic() {};
		virtual ~MyMusic() {};

		inline bool loadFromFile(const std::string& filePath)
		{
			if (!this->openFromFile(filePath))
				return false;
			return true;
		}
	private:
	};
}

class resourceSystem
{
public:
	resourceSystem() = delete;
	resourceSystem(const resourceSystem&) = delete;
	resourceSystem(const resourceSystem&&) = delete;
	virtual ~resourceSystem() { clear(); };

	template<class T>
	static inline const void add(const std::string& id, const std::string& filePath)
	{
		m_resources[id].emplace<T>();

		if (!std::get<T>(m_resources[id]).loadFromFile(filePath))
			throw "Cannot load texture...\n";
	};

	template<class T>
	[[nodiscard]]static inline const T& c_get(const std::string& id) { return std::get<T>(m_resources.at(id)); };

	template<class T>
	[[nodiscard]] static inline T& get(const std::string& id) { return std::get<T>(m_resources.at(id)); };

	static inline void clear() { m_resources.clear(); };
private:
	static std::unordered_map<std::string, std::variant<sf::Texture, sf::Font, sf::SoundBuffer, sf::MyMusic>> m_resources;
};