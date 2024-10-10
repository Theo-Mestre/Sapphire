#ifndef PROFILLING_TIMER_H
#define PROFILLING_TIMER_H

#include <chrono>

namespace sph
{
	namespace Profiling
	{
		using TimeStamp = float;

		class FrameData
		{
		public:
			FrameData(const char* _name, TimeStamp _start, TimeStamp _end)
				: Name(_name)
				, Start(_start)
				, End(_end)
			{	}

			const char* Name = "";
			TimeStamp Start = 0.0f;
			TimeStamp End = 0.0f;
		};

		template<typename OnStopCallback>
		class ScopedTimer
		{
		public:
			ScopedTimer(const char* _text, OnStopCallback&& _callback)
				: m_text(_text)
				, m_callback(std::forward<OnStopCallback>(_callback))
				, m_start(std::chrono::high_resolution_clock::now())
				, m_stopped(false)
			{	}

			~ScopedTimer()
			{
				if (IsStopped() == false)
				{
					Stop();
				}
			}

			void Stop()
			{
				auto end = std::chrono::high_resolution_clock::now().time_since_epoch().count();
				auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start).time_since_epoch().count();

				m_stopped = true;

				FrameData fd(m_text, (float)start, (float)end);
				m_callback(std::move(fd));
			}

			bool IsStopped() const { return m_stopped; }
		private:
			const char* m_text;
			OnStopCallback m_callback;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
			bool m_stopped = false;
		};
	}
}
#endif