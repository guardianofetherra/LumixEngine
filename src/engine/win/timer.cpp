#include "engine/lumix.h"
#include "engine/iallocator.h"
#include "engine/timer.h"

#include "engine/win/simple_win.h"

namespace Lumix
{


struct TimerImpl final : public Timer
{
	explicit TimerImpl(IAllocator& allocator)
		: m_allocator(allocator)
	{
		QueryPerformanceFrequency(&m_frequency);
		QueryPerformanceCounter(&m_last_tick);
		m_first_tick = m_last_tick;
	}


	float getTimeSinceStart() override
	{
		LARGE_INTEGER tick;
		QueryPerformanceCounter(&tick);
		float delta =
			static_cast<float>((double)(tick.QuadPart - m_first_tick.QuadPart) / (double)m_frequency.QuadPart);
		return delta;
	}


	u64 getRawTimeSinceStart() override
	{
		LARGE_INTEGER tick;
		QueryPerformanceCounter(&tick);
		return tick.QuadPart - m_first_tick.QuadPart;
	}


	float getTimeSinceTick() override
	{
		LARGE_INTEGER tick;
		QueryPerformanceCounter(&tick);
		float delta = static_cast<float>((double)(tick.QuadPart - m_last_tick.QuadPart) / (double)m_frequency.QuadPart);
		return delta;
	}

	float tick() override
	{
		LARGE_INTEGER tick;
		QueryPerformanceCounter(&tick);
		float delta = static_cast<float>((double)(tick.QuadPart - m_last_tick.QuadPart) / (double)m_frequency.QuadPart);
		m_last_tick = tick;
		return delta;
	}

	IAllocator& m_allocator;
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_last_tick;
	LARGE_INTEGER m_first_tick;
};


u64 Timer::getFrequency()
{
	static u64 freq = []() {
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		return f.QuadPart;
	}();
	return freq;
}


u64 Timer::getRawTimestamp()
{
	LARGE_INTEGER tick;
	QueryPerformanceCounter(&tick);
	return tick.QuadPart;
}


Timer* Timer::create(IAllocator& allocator)
{
	return LUMIX_NEW(allocator, TimerImpl)(allocator);
}


void Timer::destroy(Timer* timer)
{
	if (!timer) return;

	LUMIX_DELETE(static_cast<TimerImpl*>(timer)->m_allocator, timer);
}


} // namespace Lumix
