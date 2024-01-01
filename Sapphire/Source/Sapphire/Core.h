#ifndef SAPPHIRE_H
#define SAPPHIRE_H

#ifdef _WIN32
#	ifdef _WIN64
#		define SPH_PLATFORM_WINDOWS
#	endif
#else
#	error Only Windows x64 is supported!
#endif

#define SPH_ENABLE_ASSERTS 1
#define SPH_ENABLE_INFO_LOG

#if SPH_ENABLE_ASSERTS
#	define ASSERT(x, ...) { if(!(x)) { LogError("Assertion Failed: {}", __VA_ARGS__); __debugbreak(); } }
#else
#	define ASSERT(x, ...)
#endif

#ifdef SPH_ENABLE_INFO_LOG
#	define Info(...)	SAPPHIRE_GET_CURRENT_LOGGER->info(__VA_ARGS__)
#else
#	define Info(...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_METHOD(method) std::bind(&method, this, std::placeholders::_1)

#pragma warning(disable : 4251)
#endif 