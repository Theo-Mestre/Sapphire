#include "sphpch.h"

#include "WindowsInput.h"

#include "Sapphire/Platform/Windows/WinWindow.h"
#include <GLFW/glfw3.h>

#define GLFW_Window static_cast<GLFWwindow*>(m_window->GetNativeWindow())

bool sph::WindowsInput::IsKeyPressedImpl(int keycode)
{
	auto state = glfwGetKey(GLFW_Window, keycode);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool sph::WindowsInput::IsMouseButtonPressedImpl(int _button)
{
	auto state = glfwGetMouseButton(GLFW_Window, _button);

	return state == GLFW_PRESS;
}

glm::vec2 sph::WindowsInput::GetMousePositionImpl()
{
	double x, y;
	glfwGetCursorPos(GLFW_Window, &x, &y);
	return { (float)x, (float)y };
}

float sph::WindowsInput::GetMouseXImpl()
{
	auto pos = GetMousePositionImpl();
	return pos.x;
}

float sph::WindowsInput::GetMouseYImpl()
{
	auto pos = GetMousePositionImpl();
	return pos.y;
}
