#include "sphpch.h"
#include "Log.h"

namespace sph
{
	spdlog::logger* Logger::s_coreLogger;
	spdlog::logger* Logger::s_clientLogger;

	void Logger::Init()
	{
		SPH_PROFILE_FUNCTION();

		if (s_initialized) return;

		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_coreLogger = spdlog::stdout_color_mt("CORE").get();
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("CLIENT").get();
		s_clientLogger->set_level(spdlog::level::trace);

		s_initialized = true;

		LogInfo("Logger initialized!");

	}

	Logger::Logger()
	{
		SPH_PROFILE_FUNCTION();

		s_coreLogger = nullptr;
		s_clientLogger = nullptr;
	}

	Logger::~Logger()
	{
		SPH_PROFILE_FUNCTION();

		s_clientLogger = nullptr;
		s_coreLogger = nullptr;

		s_initialized = false;
	}
}