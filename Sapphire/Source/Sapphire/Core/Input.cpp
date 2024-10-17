#include "sphpch.h"
#include <GLFW/glfw3.h>

#include "Input.h"
#include "Sapphire/Window/Window.h"

static GLFWwindow* m_window;

namespace sph
{
	void Input::Init(Window* _window)
	{
		m_window = static_cast<GLFWwindow*>(_window->GetNativeWindow());
	}

	bool Input::IsKeyPressed(int keycode)
	{
		auto state = glfwGetKey(m_window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int _button)
	{
		auto state = glfwGetMouseButton(m_window, _button);

		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		double x, y;
		glfwGetCursorPos(m_window, &x, &y);
		return { (float)x, (float)y };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}