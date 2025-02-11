#pragma once
#include <string>

namespace sph
{
	struct ProfileResult
	{
		std::string Name;
		int64_t Start;
		int64_t End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		static void Init();
		static void Shutdown();

		static void BeginSession(const std::string& _name, const std::string& _filepath = "Results.json");
		static void EndSession();

		static void WriteProfile(const ProfileResult& _result);
		static void WriteHeader();
		static void WriteFooter();
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name);
		~InstrumentationTimer();

		void Stop();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		const char* m_name;
		bool m_stopped;
	};
}
#define SPH_PROFILE_ENABLED true

#if SPH_PROFILE_ENABLED
#define SPH_INIT_PROFILING() ::sph::Instrumentor::Init()
#define SPH_SHUTDOWN_PROFILING() ::sph::Instrumentor::Shutdown()
#define SPH_PROFILE_BEGIN_SESSION(name, filepath) ::sph::Instrumentor::BeginSession(name, "Profiling/" filepath)
#define SPH_PROFILE_END_SESSION() ::sph::Instrumentor::EndSession()
#define SPH_PROFILE_SCOPE(name) ::sph::InstrumentationTimer timer##__LINE__(name)
#define SPH_PROFILE_FUNCTION() SPH_PROFILE_SCOPE(__FUNCSIG__)
#else
#define SPH_INIT_PROFILING()
#define SPH_SHUTDOWN_PROFILING()
#define SPH_PROFILE_BEGIN_SESSION(name, filepath)
#define SPH_PROFILE_END_SESSION()
#define SPH_PROFILE_SCOPE(name)
#define SPH_PROFILE_FUNCTION()
#endif