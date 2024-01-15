#include "sphpch.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Sapphire/Core.h"
#include "OpenGLContext.h" 
#include "Sapphire/Log.h"


sph::OpenGLContext::OpenGLContext(GLFWwindow* _window)
	: GraphicsContext(_window)
{
}

sph::OpenGLContext::~OpenGLContext()
{
}

void sph::OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_windowHandle);
	int gladLoadResult = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ASSERT(gladLoadResult, "Failed to initialize Glad!");

	Info("OpenGL Info:");
	Info("  Vendor: {}", (char*)glGetString(GL_VENDOR));
	Info("  Renderer: {}", (char*)glGetString(GL_RENDERER));
	Info("  Version: {}", (char*)glGetString(GL_VERSION));
}

void sph::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_windowHandle);
}
