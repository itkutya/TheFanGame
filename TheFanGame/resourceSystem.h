#pragma once

#include <unordered_map>
#include <chrono>
#include <string>
#include <variant>
#include <vector>
#include <future>
#include <mutex>
#include <utility>
#include <stdlib.h>
#include <stdio.h>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/SFML/Graphics.hpp"
		#include "SFML64/SFML/Audio.hpp"
	#else
		#include "SFML32/Graphics.hpp"
		#include "SFML32/Audio.hpp"
	#endif
#endif

class resourceSystem
{
private:
	typedef std::variant<sf::Texture, sf::Font, sf::SoundBuffer, std::unique_ptr<sf::Music>> Resources;

	std::mutex m_mutex;
	template<class T>
	inline void addToResourceMap(std::unordered_map<std::string, Resources>* resources, std::string id, std::string filePath) noexcept
	{
		if constexpr (requires { T().loadFromFile(filePath); })
		{
			T resource = T();
			if(!resource.loadFromFile(filePath))
				std::printf("Cannot load from file: %s", filePath.c_str());

			std::lock_guard<std::mutex> lock(this->m_mutex);
			resources->insert(std::make_pair(id, Resources().emplace<T>(std::move(resource))));
		}
		else if constexpr (requires { T().openFromFile(filePath); })
		{
			std::lock_guard<std::mutex> lock(this->m_mutex);
			auto temp = std::make_unique<T>();
			if (!temp->openFromFile(filePath))
				std::printf("Cannot open file: %s", filePath.c_str());

			resources->insert(std::make_pair(id, Resources()));
			(*resources)[id].emplace<std::unique_ptr<T>>(std::move(temp));
		}
	};
public:
	resourceSystem() = default;
	resourceSystem(const resourceSystem&) = delete;
	resourceSystem(const resourceSystem&&) = delete;
	resourceSystem& operator=(resourceSystem& other) = delete;
	resourceSystem& operator=(const resourceSystem& other) = delete;
	virtual ~resourceSystem() 
	{
		this->m_resources.clear();
		for (auto& future : this->m_future)
			future.wait();
		this->m_future.clear();
	};

	template<class T>
	inline const std::future<void>& add(const std::string& id, const std::string& filePath)
	{
		return this->m_future.emplace_back(std::async(std::launch::async, &resourceSystem::addToResourceMap<T>, this, &this->m_resources, std::move(id), std::move(filePath))); 
	};

	const bool wait()
	{
		for (auto& future : this->m_future)
		{
			using namespace std::chrono_literals;
			if (future.wait_for(0ms) != std::future_status::ready)
				return true;
		}
		return false;
	}

	void release(const std::string& id) noexcept { this->m_resources.erase(id); };

	template<class T>
	[[nodiscard]] inline const T& c_get(const std::string& id) { return std::get<T>(this->m_resources.at(id)); };

	template<class T>
	[[nodiscard]] inline T& get(const std::string& id) { return std::get<T>(this->m_resources.at(id)); };
private:
	std::unordered_map<std::string, Resources> m_resources;
	std::vector<std::future<void>> m_future;
};