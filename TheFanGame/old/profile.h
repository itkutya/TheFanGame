#pragma once

#include <string>
#include <cstdint>

constexpr std::uint16_t MAX_USERNAME_LENGTH = 16;

class profile
{
public:
	profile() noexcept = delete;
	profile(const profile&) noexcept = delete;
	profile(const profile&&) noexcept = delete;
	profile& operator=(profile& other) noexcept = delete;
	profile& operator=(const profile& other) noexcept = delete;
	virtual ~profile() noexcept = default;

	inline static std::string name = "Unkown";
	inline static float xp = 0.f;
	inline static float xp_cap = 100.f;
	inline static int lvl = 1;
	inline static std::uint32_t currency = 0;
private:
};