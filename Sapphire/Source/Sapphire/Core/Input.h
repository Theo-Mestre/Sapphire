#ifndef SPH_INPUT_H
#define SPH_INPUT_H

#include "Core.h"

namespace sph
{
	class Window;

	class  Input
	{
	public:
		static bool IsKeyPressed(int _key);
		static bool IsMouseButtonPressed(int _button);

		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		static void Init(Window* _window);
	};
}
#endif