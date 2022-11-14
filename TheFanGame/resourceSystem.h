#pragma once

#include "includes.h"

class resourceSystem
{
private:
	typedef std::variant<sf::Texture, sf::Font, sf::SoundBuffer, std::unique_ptr<sf::Music>> Resources;

	template<class T>
	static inline void addToResourceMap(std::unordered_map<std::string, Resources>& resources, const std::string&& id, const std::string&& filePath) noexcept
	{
		if constexpr (requires { T().loadFromFile(filePath); })
		{
			T resource = T();
			if (!resource.loadFromFile(filePath))
				std::printf("Cannot load from file: %s", filePath.c_str());

			std::lock_guard<std::mutex> lock(m_mutex);
			resources.emplace(std::move(id), std::move(resource));
		}
		else if constexpr (requires { T().openFromFile(filePath); })
		{
			auto temp = std::make_unique<T>();
			std::lock_guard<std::mutex> lock(m_mutex);
			if (!temp->openFromFile(filePath))
				std::printf("Cannot open file: %s", filePath.c_str());

			resources.emplace(std::move(id), std::move(temp));
		}
	};
public:
	resourceSystem() = delete;
	resourceSystem(const resourceSystem&) = delete;
	resourceSystem(const resourceSystem&&) = delete;
	resourceSystem& operator=(resourceSystem& other) = delete;
	resourceSystem& operator=(const resourceSystem& other) = delete;
	virtual ~resourceSystem() { cleanUp(); };

	static inline const void cleanUp() noexcept
	{
		for (auto& future : m_future)
			future.wait();
		m_future.clear();
		m_resources.clear();
	};

	template<class T>
	static inline const std::future<void>& add(const std::string& id, const std::string& filePath) noexcept
	{
		return m_future.emplace_back(std::async(std::launch::async, &resourceSystem::addToResourceMap<T>, std::ref(m_resources), std::move(id), std::move(filePath)));
	};

	static inline const void wait() noexcept
	{
		for (auto& future : m_future)
		{
			using namespace std::chrono_literals;
			while (future.wait_for(0ms) != std::future_status::ready)
				continue;
		}
		m_future.clear();
	}

	static inline const void release(const std::string& id) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_resources.erase(id);
	};

	template<class T>
	[[nodiscard]] static inline const T& c_get(const std::string& id) noexcept { return std::get<T>(m_resources[id]); };
	template<class T>
	[[nodiscard]] static inline T& get(const std::string& id) noexcept { return std::get<T>(m_resources[id]); };
private:
	static inline std::unordered_map<std::string, Resources> m_resources;
	static inline std::vector<std::future<void>> m_future;
	static inline std::mutex m_mutex;
};