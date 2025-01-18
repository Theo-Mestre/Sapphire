#include "sphpch.h"
#include "Time.h"

#include <GLFW/glfw3.h>

namespace sph
{
	void Time::Update()
	{
		SPH_PROFILE_FUNCTION();

		GameTime = (float)glfwGetTime();

		UnscaledDeltaTime = GameTime - LastTime;
		DeltaTime = UnscaledDeltaTime * TimeScale;

		LastTime = GameTime;
	}
}