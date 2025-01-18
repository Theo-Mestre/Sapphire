#include "sphpch.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "OpenGLContext.h" 
#include "Sapphire/Core/Core.h"
#include "Sapphire/Core/Log.h"


sph::OpenGLContext::OpenGLContext(GLFWwindow* _window)
	: GraphicsContext(_window)
{
	SPH_PROFILE_FUNCTION();
}

sph::OpenGLContext::~OpenGLContext()
{
	SPH_PROFILE_FUNCTION();
}

void sph::OpenGLContext::Init()
{
	SPH_PROFILE_FUNCTION();

	glfwMakeContextCurrent(m_windowHandle);
	int gladLoadResult = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ASSERT(gladLoadResult, "Failed to initialize Glad!");

	Info("OpenGL Info:");
	Info("  Vendor: {}", (char*)glGetString(GL_VENDOR));
	Info("  Renderer: {}", (char*)glGetString(GL_RENDERER));
	Info("  Version: {}", (char*)glGetString(GL_VERSION));

#ifdef SPH_ENABLE_ASSERTS
	int versionMajor;
	int versionMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

	ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Sapphire requires at least OpenGL version 4.5!");
#endif
}

void sph::OpenGLContext::SwapBuffers()
{
	SPH_PROFILE_FUNCTION();

	glfwSwapBuffers(m_windowHandle);
}
