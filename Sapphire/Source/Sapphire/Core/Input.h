#ifndef SPH_INPUT_H
#define SPH_INPUT_H

#include "Sapphire/Core/Core.h"

namespace sph
{
	class Window;

	class  Input
	{
	public:
		static void Init(Window* _window);

		static bool IsKeyPressed(int _key);
		static bool IsMouseButtonPressed(int _button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
#endif