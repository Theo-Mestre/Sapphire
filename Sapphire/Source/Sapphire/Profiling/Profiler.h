#ifndef SPH_PROFILING_PROFILER_H
#define SPH_PROFILING_PROFILER_H

#include "Sapphire/Profiling/Timer.h"

namespace sph
{
	namespace Profiling
	{
		struct FrameData
		{
			uint32_t FrameNumber = -1;
			TimeStamp StartTime = 0.0f;
			TimeStamp EndTime = 0.0f;
			ProfileResults Results;
		};

		struct SessionData
		{
			const char* Name = "";
			TimeStamp StartTime = 0.0f;
			TimeStamp EndTime = 0.0f;
			std::vector<FrameData> Frames;
		};

		class Profiler
		{
		private:
			Profiler();
			~Profiler();

		public:
			void BeguinFrame();
			void EndFrame();
			uint32_t GetFrameNumber() const { return m_frameCount; }

			void BeginSession(const char* _name);
			void EndSession();
			const char* GetSessionName() const { return m_currentSession != nullptr ? m_currentSession->Name : ""; }

			void AddResult(ProfileResult&& _result);

			void StartRecording() { m_recording = true; }
			void StopRecording() { m_recording = false; }
			bool IsRecording() const { return m_recording; }

			static Profiler& Instance() { static Profiler instance; return instance; }

		private:
			void SaveData();

		private:
			bool m_recording;
			uint32_t m_frameCount;

			FrameData* m_currentFrame;
			std::vector<FrameData> m_framesData;

			SessionData* m_currentSession;
			std::vector<SessionData> m_sessions;
		};
	}
}

#define SPH_PROFILING_ENABLED

#ifdef SPH_PROFILING_ENABLED
#define PROFILE_SCOPE(name) ::sph::Profiling::ScopedTimer timer##__LINE__(name, [&](::sph::Profiling::ProfileResult&& result) { ::sph::Profiling::Profiler::Instance().AddResult(std::move(result)); })
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)

#define PROFILE_BEGIN_SESSION(name) ::sph::Profiling::Profiler::Instance().BeginSession(name)
#define PROFILE_END_SESSION() ::sph::Profiling::Profiler::Instance().EndSession()

#define PROFILE_START_RECORDING() ::sph::Profiling::Profiler::Instance().StartRecording()
#define PROFILE_STOP_RECORDING() ::sph::Profiling::Profiler::Instance().StopRecording()

#define PROFILE_BEGIN_FRAME() ::sph::Profiling::Profiler::Instance().BeguinFrame()
#define PROFILE_END_FRAME() ::sph::Profiling::Profiler::Instance().EndFrame()

#define PROFILE_GET_FRAME_NUMBER() ::sph::Profiling::Profiler::Instance().GetFrameNumber()
#define PROFILE_GET_SESSION_NAME() ::sph::Profiling::Profiler::Instance().GetSessionName()

#define PROFILE_SAVE_DATA() ::sph::Profiling::Profiler::Instance().SaveData()

#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION() 
#define PROFILE_BEGIN_SESSION(name)
#define PROFILE_END_SESSION() 
#define PROFILE_START_RECORDING() 
#define PROFILE_STOP_RECORDING() 
#define PROFILE_BEGIN_FRAME() 
#define PROFILE_END_FRAME() 
#define PROFILE_GET_FRAME_NUMBER() 
#define PROFILE_GET_SESSION_NAME() 
#define PROFILE_SAVE_DATA() 
#endif
#endif