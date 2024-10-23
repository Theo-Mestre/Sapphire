#include "sphpch.h"

#include <GLFW/glfw3.h>

#include "WinWindow.h"

#include "Sapphire/Core/Core.h"
#include "Sapphire/Core/Log.h"

#include "Sapphire/Events/ApplicationEvent.h"
#include "Sapphire/Events/KeyEvent.h"
#include "Sapphire/Events/MouseEvent.h"

#include "Sapphire/Platform/OpenGL/OpenGLContext.h"

namespace sph
{
	static void GLFWErrorCallback(int _error, const char* _description)
	{
		LogError("GLFW Error {}: {}", _error, _description);
	}

	WinWindow::WinWindow(const WindowProperties& _properties)
		: m_window(nullptr)
	{
		Init(_properties);
	}

	WinWindow::~WinWindow()
	{
		Shutdown();
	}

	void WinWindow::OnUpdate(void)
	{
		glfwPollEvents();
		m_context->SwapBuffers();
	}

	void WinWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
	}

	void WinWindow::Init(const WindowProperties& _properties)
	{
		// Set window data properties
		m_data.Title = _properties.Title;
		m_data.Width = _properties.Width;
		m_data.Height = _properties.Height;
		m_data.EventCallback = _properties.EventCallback;

		Info("Creating window {0} ({1}, {2})", _properties.Title, _properties.Width, _properties.Height);

		if (!s_GLFWInitialized)
		{
			// Initialize GLFW
			int glfwInitResult = glfwInit();
			ASSERT(glfwInitResult, "Failed to initialize GLFW!");

			// Set GLFW error callback and set initialized flag to true
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		// Create a GLFWwindow object that we can use for GLFW's functions
		m_window = glfwCreateWindow(_properties.Width, _properties.Height, _properties.Title.c_str(), nullptr, nullptr);
		ASSERT(m_window, "Failed to create a GLFW window!");

		// Create the graphics context
		m_context = new OpenGLContext(m_window);
		m_context->Init();

		glfwSetWindowUserPointer(m_window, &m_data);

		InitEventCallbacks();
	}

	void WinWindow::InitEventCallbacks(void)
	{
		// Close window event
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* _window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		// Resize window event
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* _window, int _width, int _height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				data.Width = _width;
				data.Height = _height;

				WindowResizeEvent event(_width, _height);
				data.EventCallback(event);
			});

		// Key event
		glfwSetKeyCallback(m_window, [](GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				switch (_action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(_key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(_key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(_key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_window, [](GLFWwindow* _window, unsigned int _key)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				KeyTypedEvent event(_key);
				data.EventCallback(event);
			});

		// Mouse button event
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* _window, int _button, int _action, int _mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				switch (_action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(_button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(_button);
					data.EventCallback(event);
					break;
				}
				}
			});

		// Mouse scroll event
		glfwSetScrollCallback(m_window, [](GLFWwindow* _window, double _xOffset, double _yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				MouseScrolledEvent event((float)_xOffset, (float)_yOffset);
				data.EventCallback(event);
			});

		// Mouse move event
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* _window, double _xPos, double _yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				MouseMovedEvent event((float)_xPos, (float)_yPos);
				data.EventCallback(event);
			});
	}

	void WinWindow::Shutdown(void)
	{
		glfwDestroyWindow(m_window);
	}

#ifdef SPH_PLATFORM_WINDOWS
	Window* Window::Create(const WindowProperties& _properties)
	{
		return new WinWindow(_properties);
	}
#endif
}