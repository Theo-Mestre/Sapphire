#include "sphpch.h"
#include <GLFW/glfw3.h>

#include "Sapphire/Core/Input.h"
#include "Sapphire/Core/Log.h"
#include "Sapphire/Core/KeyCode.h"
#include "Sapphire/Core/MouseButton.h"
#include "Sapphire/Platform/Windows/WinWindow.h"

static GLFWwindow* s_window;

bool sph::Input::IsKeyPressed(KeyCode::KeyCode keycode)
{
	SPH_PROFILE_FUNCTION();

	auto state = glfwGetKey(s_window, keycode);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool sph::Input::IsMouseButtonPressed(Mouse::Button _button)
{
	SPH_PROFILE_FUNCTION();

	auto state = glfwGetMouseButton(s_window, _button);

	return state == GLFW_PRESS;
}

glm::vec2 sph::Input::GetMousePosition()
{
	SPH_PROFILE_FUNCTION();

	double x, y;
	glfwGetCursorPos(s_window, &x, &y);
	return glm::vec2(x, y);
}

float sph::Input::GetMouseX()
{
	SPH_PROFILE_FUNCTION();

	return GetMousePosition().x;
}

float sph::Input::GetMouseY()
{
	SPH_PROFILE_FUNCTION();

	return GetMousePosition().y;
}

void sph::Input::Init(Window* _window)
{
	SPH_PROFILE_FUNCTION();

	s_window = static_cast<GLFWwindow*>(_window->GetNativeWindow());
}