#ifndef ENGINE_TIME_H
#define ENGINE_TIME_H

#include "Sapphire/Core.h"

class SPH_API Time
{
public:
	static void Update();

	inline static float TimeScale = 1.0f;

	inline static float DeltaTime = 0.0f;
	inline static float UnscaledDeltaTime = 0.0f;

	inline static float CurrentTime = 0.0f;
	inline static float LastTime = 0.0f;
	inline static float GameTime = 0.0f;
private:
};
#endif