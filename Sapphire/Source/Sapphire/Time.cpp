#include "sphpch.h"
#include "Time.h"

#include <GLFW/glfw3.h>

namespace sph
{
	void Time::Update()
	{
		GameTime = glfwGetTime();

		UnscaledDeltaTime = GameTime - LastTime;
		DeltaTime = UnscaledDeltaTime * TimeScale;

		LastTime = GameTime;
	}
}