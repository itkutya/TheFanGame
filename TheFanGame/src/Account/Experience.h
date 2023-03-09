#pragma once

#include <cstdint>

class Experience
{
public:
	explicit Experience(float value = 0.f) noexcept;
	explicit Experience(std::uint64_t lvl, float xp, float xpcap) noexcept;
	virtual ~Experience() noexcept = default;

	[[nodiscard]] const float getProgress() noexcept;
	[[nodiscard]] const std::uint64_t& getLevel() noexcept;
	[[nodiscard]] const float& getCurrentXP() noexcept;
	[[nodiscard]] const float& getCurrentXPCap() noexcept;

	const float& add(float value) noexcept;
private:
	std::uint64_t m_lvl = 1;
	float m_xp = 0.f;
	float m_xpcap = 100.f;
};

