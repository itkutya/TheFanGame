#pragma once

#include <cstdint>

class Experience
{
public:
	explicit Experience(const float& value = 0.f) noexcept;
	explicit Experience(const std::uint64_t&& lvl, const float&& xp, const float&& xpcap) noexcept;
	virtual ~Experience() noexcept = default;

	const float getProgress() noexcept;
	const std::uint64_t& getLevel() noexcept;

	const float& add(const float& value) noexcept;
private:
	std::uint64_t m_lvl = 1;
	float m_xp = 0.f;
	float m_xpcap = 100.f;
};

