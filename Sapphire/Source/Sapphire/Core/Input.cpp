#include "sphpch.h"
#include <GLFW/glfw3.h>

#include "Sapphire/Core/Input.h"
#include "Sapphire/Core/Log.h"
#include "Sapphire/Platform/Windows/WinWindow.h"


static GLFWwindow* s_window;

bool sph::Input::IsKeyPressed(int keycode)
{
	auto state = glfwGetKey(s_window, keycode);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool sph::Input::IsMouseButtonPressed(int _button)
{
	auto state = glfwGetMouseButton(s_window, _button);

	return state == GLFW_PRESS;
}

glm::vec2 sph::Input::GetMousePosition()
{
	double x, y;
	glfwGetCursorPos(s_window, &x, &y);
	return glm::vec2(x, y);
}

float sph::Input::GetMouseX()
{
	return GetMousePosition().x;
}

float sph::Input::GetMouseY()
{
	return GetMousePosition().y;
}

void sph::Input::Init(Window* _window)
{
	s_window = static_cast<GLFWwindow*>(_window->GetNativeWindow());
}