#ifndef SPH_LOG_H
#define SPH_LOG_H

#pragma warning(push, 0) // Disabling spdlod warnings
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

#include "Sapphire/Core/Core.h"

namespace sph
{
	class  Logger
	{
	public:
		static void Init();

		static auto& GetSapphireLogger() { return s_coreLogger; }
		static auto& GetClientLogger() { return s_clientLogger; }

		inline static bool s_initialized = false;

	private:
		Logger();
		~Logger();

		static spdlog::logger* s_coreLogger;
		static spdlog::logger* s_clientLogger;
	};

}
#ifdef SPH_BUILD
#	define SAPPHIRE_GET_CURRENT_LOGGER ::sph::Logger::GetSapphireLogger()
#else
#	define SAPPHIRE_GET_CURRENT_LOGGER ::sph::Logger::GetClientLogger()
#endif

#ifndef DIST
#	define LogTrace(...)	SAPPHIRE_GET_CURRENT_LOGGER->trace(__VA_ARGS__)
#	define LogDebug(...)	SAPPHIRE_GET_CURRENT_LOGGER->debug(__VA_ARGS__)
#	define LogInfo(...)		SAPPHIRE_GET_CURRENT_LOGGER->info(__VA_ARGS__)
#	define LogWarn(...)		SAPPHIRE_GET_CURRENT_LOGGER->warn(__VA_ARGS__)
#	define LogError(...)	SAPPHIRE_GET_CURRENT_LOGGER->error(__VA_ARGS__)
#	define LogCritical(...)	SAPPHIRE_GET_CURRENT_LOGGER->critical(__VA_ARGS__)
#else
#	define LogTrace(...)	
#	define LogDebug(...)	
#	define LogInfo(...)	
#	define LogWarn(...)	
#	define LogError(...)	
#	define LogCritical(...)	
#endif

#endif