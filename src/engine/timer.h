#pragma once


#include "engine/engine.h"


namespace Lumix
{
	class LUMIX_ENGINE_API Timer
	{
		public:
			virtual ~Timer() {}

			virtual float tick() = 0;
			virtual float getTimeSinceStart() = 0;
			virtual float getTimeSinceTick() = 0;
			virtual u64 getRawTimeSinceStart() = 0;

			static u64 getRawTimestamp();
			static u64 getFrequency();
			static Timer* create(IAllocator& allocator);
			static void destroy(Timer* timer);
	};

	class ScopedTimer
	{
	public:
		ScopedTimer(const char* name, IAllocator& allocator)
			: m_name(name)
			, m_timer(Timer::create(allocator))
		{

		}

		~ScopedTimer()
		{
			Timer::destroy(m_timer);
		}

		float getTimeSinceStart()
		{
			return m_timer->getTimeSinceStart();
		}

		const char* getName() const { return m_name; }

	private:
		const char* m_name;
		Timer* m_timer;
	};
} // namespace Lumix
