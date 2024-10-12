#ifndef SPH_PROFILING_PROFILER_H
#define SPH_PROFILING_PROFILER_H

#include "Sapphire/Profiling/Timer.h"

namespace sph
{
	namespace Profiling
	{
		class Profiler
		{
		public:
			Profiler();
			~Profiler();

			void AddResult(FrameData&& _result);

			void StartFrame();
			void EndFrame();

			void SaveData();

			static Profiler& Instance()
			{
				static Profiler instance;
				return instance;
			}
		private:
			std::vector<FrameData> m_frameResults;
			std::ofstream m_file;

			uint32_t m_frameNumber = 0;
		};
	}
}

#ifdef SPH_PROFILING_ENABLED
#define PROFILE_SCOPE(name) ::sph::Profiling::ScopedTimer timer##__LINE__(name, [&](::sph::Profiling::FrameData&& result) { ::sph::Profiling::Profiler::Instance().AddResult(std::move(result)); })
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)

#define PROFILE_BEGIN_FRAME() ::sph::Profiling::Profiler::Instance().StartFrame()
#define PROFILE_END_FRAME() ::sph::Profiling::Profiler::Instance().EndFrame()

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