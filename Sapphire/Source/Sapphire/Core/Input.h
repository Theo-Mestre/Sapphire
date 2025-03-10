#ifndef SPH_INPUT_H
#define SPH_INPUT_H

#include "Sapphire/Core/Core.h"
#include "Sapphire/Core/KeyCode.h"
#include "Sapphire/Core/MouseButton.h"

namespace sph
{
	class Window;

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode::KeyCode _key);
		static bool IsMouseButtonPressed(Mouse::Button _button);

		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		static void Init(Window* _window);
	};
}
#endif