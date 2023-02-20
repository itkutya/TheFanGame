#include "Timer.h"

Timer::Timer() { this->Reset(); }

Timer::~Timer() {}

const void Timer::Reset() { this->m_start = std::chrono::high_resolution_clock::now(); }

auto Timer::Elapsed() { return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->m_start).count(); }

ScopedTimer::ScopedTimer(const std::string& name, const float& treshold) : m_name(name), m_treshold(treshold) {}

ScopedTimer::~ScopedTimer()
{
	auto time = this->m_timer.Elapsed();
	if (time > this->m_treshold)
		std::cout << this->m_name << ": " << time << " us." << std::endl;
}