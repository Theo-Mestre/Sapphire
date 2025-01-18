#include "sphpch.h"

#include "Profiler.h"

#include "Sapphire/Core/Log.h"
#include "Sapphire/Core/Core.h"

namespace sph
{
	struct InstrumentorData
	{
		InstrumentationSession* CurrentSession = nullptr;
		std::ofstream OutputStream;
		int ProfileCount = 0;
	};

	static InstrumentorData* s_Data = nullptr;

	void Instrumentor::Init()
	{
		s_Data = new InstrumentorData();
	}

	void Instrumentor::Shutdown()
	{
		if (s_Data->CurrentSession)
		{
			EndSession();
		}

		delete s_Data;
		s_Data = nullptr;
	}

	void Instrumentor::BeginSession(const std::string& _name, const std::string& _filepath)
	{
		if (s_Data->CurrentSession)
		{
			LogWarn("Instrumentor::BeginSession('{0}') when session '{1}' already open.", _name, s_Data->CurrentSession->Name);
			EndSession();
		}

		s_Data->OutputStream.open(_filepath, std::ios::out);
		WriteHeader();
		s_Data->CurrentSession = new InstrumentationSession{ _name };
	}

	void Instrumentor::EndSession()
	{
		WriteFooter();
		s_Data->OutputStream.close();
		delete s_Data->CurrentSession;
		s_Data->CurrentSession = nullptr;
		s_Data->ProfileCount = 0;
	}

	void Instrumentor::WriteProfile(const ProfileResult& _result)
	{
		if (s_Data->ProfileCount++ > 0)
		{
			s_Data->OutputStream << ",";
		}

		std::string name = _result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		s_Data->OutputStream << "{";
		s_Data->OutputStream << "\"cat\":\"function\",";
		s_Data->OutputStream << "\"dur\":" << (_result.End - _result.Start) << ',';
		s_Data->OutputStream << "\"name\":\"" << name << "\",";
		s_Data->OutputStream << "\"ph\":\"X\",";
		s_Data->OutputStream << "\"pid\":0,";
		s_Data->OutputStream << "\"tid\":" << _result.ThreadID << ",";
		s_Data->OutputStream << "\"ts\":" << _result.Start;
		s_Data->OutputStream << "}";

		s_Data->OutputStream.flush();
	}

	void Instrumentor::WriteHeader()
	{
		s_Data->OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		s_Data->OutputStream.flush();
	}

	void Instrumentor::WriteFooter()
	{
		s_Data->OutputStream << "]}";
		s_Data->OutputStream.flush();
	}

	InstrumentationTimer::InstrumentationTimer(const char* name)
		: m_name(name), m_stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	InstrumentationTimer::~InstrumentationTimer()
	{
		if (!m_stopped)
			Stop();
	}

	void InstrumentationTimer::Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		int64_t start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		int64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::WriteProfile({ m_name, start, end, threadID });

		m_stopped = true;
	}
}