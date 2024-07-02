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
#	define Info(...)	LogInfo(__VA_ARGS__)
#else
#	define Info(...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_METHOD(method) std::bind(&method, this, std::placeholders::_1)

#include <memory>

namespace sph
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#pragma warning(disable : 4251)
#endif 