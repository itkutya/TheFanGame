#include "Experience.h"

Experience::Experience(float value) noexcept
{
	this->m_xp = value;
	while (this->m_xp >= this->m_xpcap)
	{
		this->m_xp -= this->m_xpcap;
		++this->m_lvl;
		this->m_xpcap += (float)this->m_lvl * 100.f;
	}
}

Experience::Experience(std::uint64_t lvl, float xp, float xpcap) noexcept
{
	this->m_lvl = lvl;
	this->m_xpcap = xpcap;
	this->m_xp = xp;
	while (this->m_xp >= this->m_xpcap)
	{
		this->m_xp -= this->m_xpcap;
		++this->m_lvl;
		this->m_xpcap += (float)this->m_lvl * 100.f;
	}
}

const float Experience::getProgress() noexcept
{
	return this->m_xp / this->m_xpcap;
}

const std::uint64_t& Experience::getLevel() noexcept
{
	return this->m_lvl;
}

const float& Experience::getCurrentXP() noexcept
{
	return this->m_xp;
}

const float& Experience::getCurrentXPCap() noexcept
{
	return this->m_xpcap;
}

const float& Experience::add(float value) noexcept
{
	this->m_xp += value;
	while (this->m_xp >= this->m_xpcap)
	{
		this->m_xp -= this->m_xpcap;
		++this->m_lvl;
		this->m_xpcap += (float)this->m_lvl * 100.f;
	}
	return this->m_xp;
}